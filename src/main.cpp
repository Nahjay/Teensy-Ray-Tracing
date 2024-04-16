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

// Define the color function
Color ray_color(const ray& r) {
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
  auto focal_length = 1.0;
  auto camera_origin = point3(0, 0, 0);

  // Calculate the horizontal and vertical vectors
  auto horizontal = Vector3(viewport_width, 0, 0);
  auto vertical = Vector3(0, -viewport_height, 0);

  // Calculate the location of the upper left pixel should be 0, 0, 0
  auto viewport_upper_left = camera_origin;

  

  tft.fillScreen(ILI9341_BLUE);
  
  // delay(1000000); // Delay to view the result for a while
}