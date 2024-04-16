#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include "vec3.h"

#define TFT_CS 10
#define TFT_DC 9

// Define the color class using the Vector3 type as the underlying data type
using Color = Vector3;

void writeColor(Color pixelColor) {
  // Convert the color to 16-bit values
  uint16_t r = uint16_t(255.999 * pixelColor.x());
  uint16_t g = uint16_t(255.999 * pixelColor.y());
  uint16_t b = uint16_t(255.999 * pixelColor.z());

  // Create an instance of the display
  Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

  // Write the color to the display
  tft.writePixel(r, g, b);
}

#endif 