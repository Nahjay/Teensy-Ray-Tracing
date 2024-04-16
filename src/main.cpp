#include <Arduino.h>
#include <Adafruit_ILI9341.h>

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

struct Vector3 {
  float x, y, z;

  // Vector addition
  Vector3 operator +(const Vector3& v) const {
    return {x + v.x, y + v.y, z + v.z};
  }

  // Vector subtraction
  Vector3 operator -(const Vector3& v) const {
    return {x - v.x, y - v.y, z - v.z};
  }

  // Scalar multiplication
  Vector3 operator *(float scalar) const {
    return {x * scalar, y * scalar, z * scalar};
  }

  // Dot product
  float dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  // Length of the vector
  float length() const {
    return sqrt(x * x + y * y + z * z);
  }

  // Cross product
  Vector3 cross(const Vector3& v) const {
    return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
  }

  // Normalize the vector
  Vector3 normalize() const {
    float len = sqrt(x * x + y * y + z * z);
    return {x / len, y / len, z / len};
  }
};

struct Sphere {
  Vector3 center;
  float radius;
  float refractiveIndex = 0; // Refractive index of the sphere (0 means opaque)
  float reflectivity = 0; // Reflectivity of the sphere (0 means no reflection)
  float transparency = 0; // Transparency of the sphere (0 means opaque)
};

struct Light {
  Vector3 position;
  float intensity;
};

// Mix two colors
uint16_t mixColors(uint16_t color1, uint16_t color2, float ratio) {
  uint8_t r1 = (color1 >> 11) & 0x1F;
  uint8_t g1 = (color1 >> 5) & 0x3F;
  uint8_t b1 = color1 & 0x1F;

  uint8_t r2 = (color2 >> 11) & 0x1F;
  uint8_t g2 = (color2 >> 5) & 0x3F;
  uint8_t b2 = color2 & 0x1F;

  uint8_t r = r1 * (1 - ratio) + r2 * ratio;
  uint8_t g = g1 * (1 - ratio) + g2 * ratio;
  uint8_t b = b1 * (1 - ratio) + b2 * ratio;

  return (r << 11) | (g << 5) | b;
}

bool intersect(const Vector3& rayOrigin, const Vector3& rayDirection, const Sphere& sphere, float& t) {
  Vector3 oc = rayOrigin - sphere.center;
  float a = rayDirection.dot(rayDirection);
  float b = 2.0 * oc.dot(rayDirection);
  float c = oc.dot(oc) - sphere.radius * sphere.radius;
  float discriminant = b*b - 4*a*c;
  if (discriminant < 0) {
    return false; // no intersection
  } else {
    float t1 = (-b - sqrt(discriminant)) / (2.0 * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0 * a);
    t = (t1 < t2 && t1 > 0) ? t1 : t2;
    return t > 0; // intersection at t
  }
}

void drawGradientBackground(uint16_t color1, uint16_t color2) {
  for (int y = 0; y < tft.height(); y++) {
    float ratio = (float)y / tft.height();
    uint16_t color = mixColors(color1, color2, ratio);
    tft.drawFastHLine(0, y, tft.width(), color);
  }
}

// Recursive function to trace a ray and calculate its color
// uint16_t traceRay(Vector3 origin, Vector3 dir, Sphere sphere, Light light, int depth) {
//   float t;
//   if (intersect(origin, dir, sphere, t)) {
//     Vector3 hit = origin + dir * t;
//     Vector3 normal = (hit - sphere.center).normalize();
//     Vector3 lightDir = (light.position - hit).normalize();
//     float distanceToLight = (light.position - hit).length();
//     float intensity = max(0.0, normal.dot(lightDir)) / (distanceToLight * distanceToLight); // Apply the inverse square law
//     intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
//     uint16_t color = intensity * ILI9341_WHITE;

//     if (depth < 3) { // Limit the recursion depth to 3
//       // Calculate the reflection ray
//       Vector3 reflectionDir = dir - normal * 2.0 * normal.dot(dir);

//       // Cast a reflection ray
//       uint16_t reflection_color = traceRay(hit + normal * 0.001f, reflectionDir, sphere, light, depth + 1);

//       // If the sphere is transparent
//       if (sphere.transparency > 0) {
//         // Calculate the refracted ray
//         float eta = 1 / sphere.refractiveIndex;
//         float cosi = -normal.dot(dir);
//         float k = 1 - eta * eta * (1 - cosi * cosi);
//         if (k < 0) {
//           // Total internal reflection, no refraction
//           color = reflection_color;
//         } else {
//           Vector3 refractedDir = dir * eta + normal * (eta * cosi - sqrt(k));

//           // Cast a refracted ray
//           uint16_t refracted_color = traceRay(hit - normal * 0.001f, refractedDir, sphere, light, depth + 1);

//           // Use the Fresnel equations to calculate the ratio of reflection to refraction
//           float R0 = (1 - sphere.refractiveIndex) / (1 + sphere.refractiveIndex);
//           R0 = R0 * R0;
//           float cosX = -normal.dot(dir);
//           if (sphere.refractiveIndex > 1) {
//             cosX = sqrt(1 - sphere.refractiveIndex * sphere.refractiveIndex * (1 - cosX * cosX));
//           }
//           float R = R0 + (1 - R0) * pow(1 - cosX, 5);

//           // Combine the reflection and refraction colors
//           color = mixColors(reflection_color, refracted_color, R);
//         }
//       } else {
//         // If the sphere is not transparent, just use the reflection color
//         color = reflection_color;
//       }
//     }

//     return color;
//   } else {
//     return ILI9341_BLACK; // Return black if the ray doesn't intersect the sphere
//   }
// }
// uint16_t traceRay(Vector3 origin, Vector3 dir, const std::vector<Sphere>& spheres, Light light, int depth) {
//   float closestT = std::numeric_limits<float>::max();
//   uint16_t closestColor = ILI9341_BLACK; // Background color

//   for (const Sphere& sphere : spheres) {
//     float t;
//     if (intersect(origin, dir, sphere, t)) {
//       Vector3 hit = origin + dir * t;
//       Vector3 normal = (hit - sphere.center).normalize();
//       Vector3 lightDir = (light.position - hit).normalize();
//       float distanceToLight = (light.position - hit).length();
//       float intensity = max(0.0, normal.dot(lightDir)) / (distanceToLight * distanceToLight); // Apply the inverse square law
//       intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
//       uint16_t color = intensity * ILI9341_WHITE;

//       if (depth < 3) { // Limit the recursion depth to 3
//         // Calculate the reflection ray
//         Vector3 reflectionDir = dir - normal * 2.0 * normal.dot(dir);

//         // Cast a reflection ray
//         uint16_t reflection_color = traceRay(hit + normal * 0.001f, reflectionDir, spheres, light, depth + 1);

//         // If the sphere is transparent
//         if (sphere.transparency > 0) {
//           // Calculate the refracted ray
//           float eta = 1 / sphere.refractiveIndex;
//           float cosi = -normal.dot(dir);
//           float k = 1 - eta * eta * (1 - cosi * cosi);
//           if (k < 0) {
//             // Total internal reflection, no refraction
//             color = reflection_color;
//           } else {
//             Vector3 refractedDir = dir * eta + normal * (eta * cosi - sqrt(k));

//             // Cast a refracted ray
//             uint16_t refracted_color = traceRay(hit - normal * 0.001f, refractedDir, spheres, light, depth + 1);

//             // Use the Fresnel equations to calculate the ratio of reflection to refraction
//             float R0 = (1 - sphere.refractiveIndex) / (1 + sphere.refractiveIndex);
//             R0 = R0 * R0;
//             float cosX = -normal.dot(dir);
//             if (sphere.refractiveIndex > 1) {
//               cosX = sqrt(1 - sphere.refractiveIndex * sphere.refractiveIndex * (1 - cosX * cosX));
//             }
//             float R = R0 + (1 - R0) * pow(1 - cosX, 5);

//             // Combine the reflection and refraction colors
//             color = mixColors(reflection_color, refracted_color, R);
//           }
//         } else {
//           // If the sphere is not transparent, just use the reflection color
//           color = reflection_color;
//         }
//       }

//       if (t < closestT) {
//         closestT = t;
//         closestColor = color;
//       }
//     }
//   }

//   return closestColor;
// }
uint16_t traceRay(Vector3 origin, Vector3 dir, const std::vector<Sphere>& spheres, Light light, int depth) {
  float closestT = std::numeric_limits<float>::max();
  uint16_t closestColor = ILI9341_BLACK; // Background color

  for (const Sphere& sphere : spheres) {
    float t;
    if (intersect(origin, dir, sphere, t)) {
      Vector3 hit = origin + dir * t;
      Vector3 normal = (hit - sphere.center).normalize();
      Vector3 lightDir = (light.position - hit).normalize();
      float distanceToLight = (light.position - hit).length();
      float intensity = max(0.0, normal.dot(lightDir)) / (distanceToLight * distanceToLight); // Apply the inverse square law
      intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
      uint16_t color = intensity * ILI9341_WHITE;

      Serial.print("Intersection at t = ");
      Serial.println(t);
      Serial.print("Intensity = ");
      Serial.println(intensity);

      if (depth < 3) { // Limit the recursion depth to 3
        // Calculate the reflection ray
        Vector3 reflectionDir = dir - normal * 2.0 * normal.dot(dir);

        // Cast a reflection ray
        uint16_t reflection_color = traceRay(hit + normal * 0.001f, reflectionDir, spheres, light, depth + 1);

        Serial.print("Reflection color = ");
        Serial.println(reflection_color, HEX);

        // If the sphere is transparent
        if (sphere.transparency > 0) {
          // Calculate the refracted ray
          float eta = 1 / sphere.refractiveIndex;
          float cosi = -normal.dot(dir);
          float k = 1 - eta * eta * (1 - cosi * cosi);
          if (k < 0) {
            // Total internal reflection, no refraction
            color = reflection_color;
          } else {
            Vector3 refractedDir = dir * eta + normal * (eta * cosi - sqrt(k));

            // Cast a refracted ray
            uint16_t refracted_color = traceRay(hit - normal * 0.001f, refractedDir, spheres, light, depth + 1);

            Serial.print("Refracted color = ");
            Serial.println(refracted_color, HEX);

            // Use the Fresnel equations to calculate the ratio of reflection to refraction
            float R0 = (1 - sphere.refractiveIndex) / (1 + sphere.refractiveIndex);
            R0 = R0 * R0;
            float cosX = -normal.dot(dir);
            if (sphere.refractiveIndex > 1) {
              cosX = sqrt(1 - sphere.refractiveIndex * sphere.refractiveIndex * (1 - cosX * cosX));
            }
            float R = R0 + (1 - R0) * pow(1 - cosX, 5);

            // Combine the reflection and refraction colors
            color = mixColors(reflection_color, refracted_color, R);
          }
        } else {
          // If the sphere is not transparent, just use the reflection color
          color = reflection_color;
        }
      }

      if (t < closestT) {
        closestT = t;
        closestColor = color;
      }
    }
  }

  return closestColor;
}

void setup() {
  // Set up the display by beginning the SPI connection
  SPI.setMOSI(TFT_MOSI);
  SPI.setSCK(TFT_CLK);

  // Sphere sphere = {{50, 50, 20}, 10}; // Center at (50,50,20) with radius 10
  // Light light = {{10, 10, 10}, 1.0}; 

  // Initialize the display
  tft.begin();

  // Set the rotation of the display
  tft.setRotation(3);

  // Draw gradient background
  // drawGradientBackground(ILI9341_BLUE, ILI9341_BLACK);
}

void loop() {

  // // Delay for 1 second
  // delay(1000);

  // Sphere sphere = {{120, 160, -100}, 20}; // Center at (50,50,20) with radius 10
  // Light light = {{10, 10, 10}, 10.0};

  // Vector3 eye = {0, 0, 0}; // Camera/eye position

  // // Clear the display
  // tft.fillScreen(ILI9341_BLACK);

  // // Draw the sphere normally
  // tft.fillCircle(sphere.center.x, sphere.center.y, sphere.radius, ILI9341_WHITE);

  // // Draw the light source
  // tft.fillCircle(light.position.x, light.position.y, 2, ILI9341_YELLOW);

  // // Draw the eye
  // tft.fillCircle(eye.x, eye.y, 2, ILI9341_BLUE);

  // // Draw the ray from the eye to the sphere
  // Vector3 rayDir = sphere.center - eye;
  // rayDir = rayDir.normalize();
  // float t;
  // if (intersect(eye, rayDir, sphere, t)) {
  //   Vector3 hit = eye + rayDir * t;
  //   tft.drawLine(eye.x, eye.y, hit.x, hit.y, ILI9341_RED);
  // }

  // // Let the raytracing begin!
  // // For each pixel in the display
  // for (int y = 0; y < tft.height(); y++) {
  //   for (int x = 0; x < tft.width(); x++) {
  //     // Generate a ray from the eye through the pixel
  //     Vector3 rayDir = {x - eye.x, y - eye.y, -eye.z};
  //     rayDir = rayDir.normalize();

  //     // Determine if this ray intersects the sphere
  //     float t;
  //     if (intersect(eye, rayDir, sphere, t)) {
  //       // If it does, calculate the color of the pixel based on the light source and the properties of the sphere
  //       Vector3 hit = eye + rayDir * t;
  //       Vector3 normal = (hit - sphere.center).normalize();
  //       Vector3 toLight = (light.position - hit).normalize();
  //       float intensity = max(0.0f, normal.dot(toLight));
  //       uint16_t color = intensity * ILI9341_WHITE;

  //       // Print the calculated color to the serial monitor
  //       Serial.println(color);

  //       // Print the calculated position to the serial monitor
  //       Serial.println("x: " + String(x) + " y: " + String(y));

  //       // Write the calculated color to the corresponding pixel on the display
  //       tft.drawPixel(x, y, color);
  //     }
  //   }
  // }
  // Delay for 1 second
  delay(1000);

  // Draw gradient background
  // drawGradientBackground(ILI9341_BLUE, ILI9341_BLACK);

  // Sphere sphere = {{40, 40, -100}, 20}; // Center at (120,160,-100) with radius 50
  // Light light = {{10, 10, 10}, 1000.0};

  // // Metallic sphere: High reflectivity, low transparency, refractive index similar to metals
  // Sphere metallicSphere = {{40, 40, -100}, 20, 0.15, 0.9, 0.1}; 

  // // Water sphere: Low reflectivity, high transparency, refractive index similar to water
  // Sphere waterSphere = {{70, 70, -100}, 20, 1.33, 0.1, 0.9}; 

  // // Create a vector of spheres
  // std::vector<Sphere> spheres = {metallicSphere, waterSphere};

  // Vector3 eye = {0, 0, 0}; // Camera/eye position

  // Light source is moved closer to the spheres and has a higher intensity
  Light light = {{0, 0, 0}, 1000000.0};

  // Metallic sphere is moved closer to the eye and has a larger radius
  Sphere metallicSphere = {{30, 30, -50}, 10, 0.2, 0.1, 0.1}; 

  // Water sphere is moved closer to the eye, has a larger radius, and is positioned so it doesn't overlap with the metallic sphere
  Sphere waterSphere = {{70, 70, -50}, 10, 1.33, 0.1, 0.8}; 

  // Create a vector of spheres
  std::vector<Sphere> spheres = {metallicSphere, waterSphere};

  // Eye position remains the same
  Vector3 eye = {0, 0, 0}; 

  // Clear the display
  tft.fillScreen(ILI9341_BLACK);

  // Draw the gradient background
  drawGradientBackground(ILI9341_BLUE, ILI9341_BLACK);

  // Draw the light source
  tft.fillCircle(light.position.x, light.position.y, 2, ILI9341_YELLOW);

  // Draw the eye
  tft.fillCircle(eye.x, eye.y, 2, ILI9341_BLACK);

  // Set the display position in front of the eye
  float displayZ = -1;


// Draw the scene using ray tracing
for (int y = 0; y < tft.height(); y++) {
  for (int x = 0; x < tft.width(); x++) {
    // Generate a ray from the eye through the pixel
    float aspectRatio = (float)tft.width() / (float)tft.height();
    Vector3 rayDir = {x / (float)tft.width(), (y / (float)tft.height()) * aspectRatio, displayZ};
    rayDir = rayDir - eye;
    rayDir = rayDir.normalize();

    // Trace the ray and get the color of the pixel
    uint16_t color = traceRay(eye, rayDir, spheres, light, 100);

    // If an intersection was found, print some debugging information
    if (color != ILI9341_BLACK) {
      Serial.println("Intersection found at pixel (" + String(x) + ", " + String(y) + ")");
      Serial.println("Color: " + String(color));
    }

    // Write the calculated color to the corresponding pixel on the display
    tft.drawPixel(x, y, color);
  }
}
  // Sphere sphere = {{120, 160, -100}, 20}; // Center at (120,160,-100) with radius 50


  // Draw the scene using ray tracing
// for (int y = 0; y < tft.height(); y++) {
//   for (int x = 0; x < tft.width(); x++) {
//     // Generate a ray from the eye through the pixel
//     float aspectRatio = (float)tft.width() / (float)tft.height();
//     Vector3 rayDir = {x / (float)tft.width(), (y / (float)tft.height()) * aspectRatio, displayZ};
//     rayDir = rayDir - eye;
//     rayDir = rayDir.normalize();

//     // Trace the ray and get the color of the pixel for each sphere
//     uint16_t colorMetal = traceRay(eye, rayDir, metallicSphere, light, 0);
//     uint16_t colorWater = traceRay(eye, rayDir, waterSphere, light, 0);

//     // Determine the closest intersection
//     float tMetal, tWater;
//     bool hitMetal = intersect(eye, rayDir, metallicSphere, tMetal);
//     bool hitWater = intersect(eye, rayDir, waterSphere, tWater);

//     uint16_t color;
//     if (hitMetal && hitWater) {
//       color = (tMetal < tWater) ? colorMetal : colorWater;
//     } else if (hitMetal) {
//       color = colorMetal;
//     } else if (hitWater) {
//       color = colorWater;
//     } else {
//       color = ILI9341_BLACK; // Background color
//     }

//     // Write the calculated color to the corresponding pixel on the display
//     tft.drawPixel(x, y, color);
//   }
// }

// // Draw the sphere using ray tracing
// for (int y = 0; y < tft.height(); y++) {
//   for (int x = 0; x < tft.width(); x++) {
//     Vector3 rayDir = {x / (float)tft.width(), y / (float)tft.height(), displayZ};
//     rayDir = rayDir - eye;
//     rayDir = rayDir.normalize();
//     float t;
//     if (intersect(eye, rayDir, sphere, t)) {
//       Vector3 hit = eye + rayDir * t;
//       Vector3 normal = (hit - sphere.center).normalize();
//       Vector3 lightDir = (light.position - hit).normalize();
//       float intensity = max(0.0, normal.dot(lightDir));
//       intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
//       uint16_t color = intensity * ILI9341_WHITE;
//       tft.drawPixel(x, y, color);

//       // // Draw every 10th ray
//       // if ((x + y) % 10 == 0) {
//       //   tft.drawLine(eye.x, eye.y, hit.x, hit.y, ILI9341_RED);
//       // }
//     }
//   }
// }
  // // Draw the sphere using ray tracing
  // for (int y = 0; y < tft.height(); y++) {
  //   for (int x = 0; x < tft.width(); x++) {
  //     float aspectRatio = (float)tft.width() / (float)tft.height();
  //   Vector3 rayDir = {x / (float)tft.width(), (y / (float)tft.height()) * aspectRatio, displayZ};
  //     rayDir = rayDir - eye;
  //     rayDir = rayDir.normalize();
  //     float t;
  //     if (intersect(eye, rayDir, sphere, t)) {
  //       Vector3 hit = eye + rayDir * t;
  //       Vector3 normal = (hit - sphere.center).normalize();
  //       Vector3 lightDir = (light.position - hit).normalize();
  //       float intensity = max(0.0, normal.dot(lightDir));
  //       intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
  //       uint16_t color = intensity * ILI9341_WHITE;
  //       tft.drawPixel(x, y, color);

  //       // Draw every 100th ray
  //       if (x % 100 == 0 && y % 100 == 0) {
  //         tft.drawLine(eye.x, eye.y, hit.x, hit.y, ILI9341_RED);
  //       }
  //     }
  //   }
  // }

  // Draw the scene using ray tracing
// for (int y = 0; y < tft.height(); y++) {
//   for (int x = 0; x < tft.width(); x++) {
//     // Generate a ray from the eye through the pixel
//     Vector3 rayDir = {x - eye.x, y - eye.y, -eye.z};
//     rayDir = rayDir.normalize();

//     // Trace the ray and get the color of the pixel
//     uint16_t color = traceRay(eye, rayDir, sphere, light, 0);

//     // Write the calculated color to the corresponding pixel on the display
//     tft.drawPixel(x, y, color);
//   }
// }



  delay(1000000); // Delay to view the result for a while
}