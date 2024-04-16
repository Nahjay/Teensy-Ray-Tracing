#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <ray_tracing.h>
#include <camera.h>

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

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

  // Create the World
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

  // Create the camera
  camera cam;
  cam.sample_per_pixel = 10;
  cam.render(tft, world);
  
  delay(1000000); // Delay to view the result for a while
}