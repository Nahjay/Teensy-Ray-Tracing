#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <vec3.h>
#include <color.h>
#include <ray.h>

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

// Defining a simple hit shpere function
double hit_sphere(const point3& center, double radius, const ray& r) {
  Vector3 oc = center - r.origin();
  auto a = r.direction().length_squared();
  auto h = dot(r.direction(), oc);
  auto c = oc.length_squared() - radius*radius;
  auto discriminant = h*h - a*c;


  if (discriminant < 0) {
      return -1.0;
  } else {
      return (h - sqrt(discriminant)) / a;
  }
}

// Define the color function
Color ray_color(const ray& r) {
  // Shaded sphere
  auto s = hit_sphere(point3(0,0,-1), 0.5, r);
  if (s > 0.0) {
    Vector3 N = unit_vector(r.at(s) - Vector3(0,0,-1));
    return 0.5*Color(N.x()+1, N.y()+1, N.z()+1);
  }

  Vector3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  Color color = (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);

  Serial.print("Ray direction: ");
  Serial.print(r.direction().x());
  Serial.print(", ");
  Serial.print(r.direction().y());
  Serial.print(", ");
  Serial.println(r.direction().z());

  Serial.print("Color: ");
  Serial.print(color.x());
  Serial.print(", ");
  Serial.print(color.y());
  Serial.print(", ");
  Serial.println(color.z());

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
  // Clear the display
  tft.fillScreen(ILI9341_BLACK);

  // Display is 320x240

  // Define the image size to fit the display
  auto aspect_ratio = tft.width() / tft.height();

  // Camera
  auto viewport_height = 2.0;
  auto viewport_width = aspect_ratio * viewport_height;
  auto camera_origin = point3(0, 0, 0);

  // Calculate the horizontal and vertical vectors
  auto horizontal = Vector3(viewport_width, 0, 0);
  auto vertical = Vector3(0, -viewport_height, 0);

  // Calculate the location of the upper left pixel should be 0, 0, 0
  auto viewport_upper_left = camera_origin - horizontal / 2 - vertical / 2 - Vector3(0, 0, 1);

  // Render the image
  for (int y = 0; y < tft.height(); y++) {
    for (int x = 0; x < tft.width(); x++) {
      auto u = double(x) / (tft.width() - 1);
      auto v = double(y) / (tft.height() - 1);

      // Calculate the ray
      ray r(camera_origin, viewport_upper_left + u * horizontal + v * vertical - camera_origin);

      // Get the color of the ray
      Color pixel_color = ray_color(r);

      // Print the pixel color to the Serial Monitor
      Serial.print("Pixel color: ");
      Serial.print(pixel_color.x());
      Serial.print(", ");
      Serial.print(pixel_color.y());
      Serial.print(", ");
      Serial.println(pixel_color.z());

      // Print the pixel itsellf to the Serial Monitor
      Serial.print("Pixel: ");
      Serial.print(x);
      Serial.print(", ");
      Serial.println(y);


      // Write the color to the display
      writeColor(x, y, pixel_color, tft);
    }
  }
  // tft.fillScreen(ILI9341_BLUE);
  
  delay(1000000); // Delay to view the result for a while
}