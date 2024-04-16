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
  uint16_t color;
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
      // float intensity = max(0.0, normal.dot(lightDir)) / (distanceToLight * distanceToLight); // Apply the inverse square law
      // intensity = min(1.0, intensity); // Clamp the intensity between 0.0 and 1.0
      // uint16_t color = intensity < 0.1 ? sphere.color : ILI9341_BLACK;

      // float lightDistance = (light.position - hitPoint).length();

      // Calculate the light distance
      Vector3 toLight = light.position - hit;
      float lightDistance = toLight.length();

       // Lighting calculation
      float intensity = std::max(0.0f, normal.dot(toLight)) / (lightDistance * lightDistance);
      intensity = std::max(0.0f, std::min(1.0f, intensity)); // Clamp intensity to [0, 1]

      // Calculate a ratio based on the distance to the light source using the inverse square law
      float distanceRatio = std::min(1.0f, lightDistance / 200.0f); // Assuming a maximum distance of 200 units

      // Use the distance ratio to mix the sphere color with another color
      uint16_t color = mixColors(sphere.color * intensity, sphere.color , distanceRatio);

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

  // Initialize the display
  tft.begin();

  // Set the rotation of the display
  tft.setRotation(3);
}

void loop() {
  delay(1000);

  // Light source is moved closer to the spheres and has a higher intensity
  Light light = {{0, 0, -22}, 1.0};

  // Metallic sphere is moved closer to the eye and has a larger radius
  // Sphere metallicSphere = {{10, 10, -20}, 10, 0.2, 0.9, 0.1}; 

  // Water sphere is moved closer to the eye, has a larger radius, and is positioned so it doesn't overlap with the metallic sphere
  // Sphere waterSphere = {{5, 5, -20}, 10, 1.33, 0.5, 0.8}; 

  Sphere metallicSphere = {{7, 7, -20}, 5, 0.2, 0.9, 0.1, ILI9341_RED}; 
  Sphere waterSphere = {{17, 22, -25}, 5, 1.33, 0.5, 0.8, ILI9341_BLUE}; 

  // Create a vector of spheres
  std::vector<Sphere> spheres = {metallicSphere, waterSphere};

  // Eye position remains the same
  Vector3 eye = {0, 0, 0}; 

  // Clear the display
  tft.fillScreen(ILI9341_BLACK);

  // Draw the gradient background
  drawGradientBackground(ILI9341_BLACK, ILI9341_BLACK);

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

  delay(1000000); // Delay to view the result for a while
}