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
  uint16_t color;
};

struct Light {
  Vector3 position;
  float intensity;
};

// Utility functions
uint16_t mixColors(uint16_t colorA, uint16_t colorB, float ratio) {
    // Mixes two colors according to a given ratio.
    // This simplistic implementation assumes each color is RGB565.
    uint32_t r = ((colorA >> 11) & 0x1F) * ratio + ((colorB >> 11) & 0x1F) * (1 - ratio);
    uint32_t g = ((colorA >> 5) & 0x3F) * ratio + ((colorB >> 5) & 0x3F) * (1 - ratio);
    uint32_t b = (colorA & 0x1F) * ratio + (colorB & 0x1F) * (1 - ratio);
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



uint16_t traceRay(Vector3 origin, Vector3 dir, Sphere sphere, Light light, int depth) {
  float t;
  if (!intersect(origin, dir, sphere, t)) {
    return ILI9341_BLACK; // Background color if no intersection
  }

  Vector3 hitPoint = origin + dir * t;
  Vector3 normal = (hitPoint - sphere.center).normalize();
  Vector3 toLight = (light.position - hitPoint).normalize();
  float lightDistance = (light.position - hitPoint).length();

  // Lighting calculation
  float intensity = std::max(0.0f, normal.dot(toLight)) / (lightDistance * lightDistance);
  intensity = std::max(0.0f, std::min(1.0f, intensity)); // Clamp intensity to [0, 1]

   // Calculate a ratio based on the distance to the light source using the inverse square law
  float distanceRatio = std::min(1.0f, lightDistance / 200.0f); // Assuming a maximum distance of 200 units

  // Use the distance ratio to mix the sphere color with another color
  // uint16_t color = mixColors(sphere.color, ILI9341_BLUE, distanceRatio);
  uint16_t color = mixColors(sphere.color * intensity, sphere.color , distanceRatio);

  // Handling reflections and refractions
  if (depth <= 3) {
    // Reflection
    Vector3 reflectionDir = dir - normal * 2.0f * normal.dot(dir);
    uint16_t reflectionColor = traceRay(hitPoint + normal * 0.001f, reflectionDir, sphere, light, depth + 1);

    // Refraction
    float eta = 1.0f / sphere.refractiveIndex;
    float cosi = -normal.dot(dir);
    float k = 1 - eta * eta * (1 - cosi * cosi);
    if (k >= 0) {
      Vector3 refractedDir = dir * eta + normal * (eta * cosi - std::sqrt(k));
      uint16_t refractionColor = traceRay(hitPoint - normal * 0.001f, refractedDir, sphere, light, depth + 1);
      color = refractionColor;
    } else {
      color = reflectionColor;
    }
  }

  return color;
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

  // Define the sphere
  Sphere sphere = {
    .center = {120, 160, 100},
    .radius = 50,
    .refractiveIndex = 2,
    .reflectivity = 0.90,
    .color = ILI9341_RED
  };

  // Define the light source
  Light light = {
    .position = {100, 100, 100}, // Moved farther
    .intensity = 1.0
  };

  // Position of the camera
  Vector3 camera = {120, 160, 0};

  // Trace rays for each pixel
  for (int y = 0; y < tft.height(); y++) {
    for (int x = 0; x < tft.width(); x++) {
      Vector3 dir = {(float)x - camera.x, (float)y - camera.y, 100 - camera.z};
      uint16_t color = traceRay(camera, dir.normalize(), sphere, light, 0);
      tft.drawPixel(x, y, color);
    }
  }  


  delay(1000000); // Delay to view the result for a while
}