#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include "vec3.h"

// Define the color class using the Vector3 type as the underlying data type
using Color = Vector3;

void writeColor(int x, int y, Color pixelColor, Adafruit_ILI9341& tft) {
  // Convert the color to 16-bit values
  uint16_t r = uint16_t(255.999 * pixelColor.x());
  uint16_t g = uint16_t(255.999 * pixelColor.y());
  uint16_t b = uint16_t(255.999 * pixelColor.z());

  // Convert the RGB color to a 16-bit color
  uint16_t color = tft.color565(r, g, b);

  // Print the color to the Serial Monitor
    Serial.print("Color: ");
    Serial.print(r);
    Serial.print(", ");
    Serial.print(g);
    Serial.print(", ");
    Serial.println(b);
  // Write the color to the display
  tft.drawPixel(x, y, color);
}

// Define a function to write the color to the display

#endif 