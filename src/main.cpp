#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <ray_tracing.h>
#include <camera.h>
#include "material.h"

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

  // Create the materials
  auto material_ground = std::make_shared<lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<lambertian>(Color(0.1, 0.2, 0.5));
  auto material_left = std::make_shared<dielectric>(1.5);
  auto material_bubble = std::make_shared<dielectric>(1/1.5);
  auto material_right = std::make_shared<metal>(Color(0.8, 0.6, 0.2), 0.0);

  // Create the spheres
  world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
  world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
  world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
  world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
  
  // Create the camera
  camera cam;
  cam.sample_per_pixel = 10;
  cam.max_depth = 50;
  cam.render(tft, world);
  
  delay(1000000); // Delay to view the result for a while
}