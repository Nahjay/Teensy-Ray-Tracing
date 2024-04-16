#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include "vec3.h"
#include "interval.h"

// Define the color class using the Vector3 type as the underlying data type
using Color = Vector3;

// Define a gamma correction function
inline double linear_to_gamma(double linear_component) {
  if (linear_component > 0) {
    return sqrt(linear_component);
  }
  return 0;
}

void writeColor(int x, int y, Color pixelColor, Adafruit_ILI9341& tft) {
  // Convert the color to 16-bit values
  uint16_t r = uint16_t(255.999 * pixelColor.x());
  uint16_t g = uint16_t(255.999 * pixelColor.y());
  uint16_t b = uint16_t(255.999 * pixelColor.z());

  // Apply gamma correction
  r = uint16_t(255.999 * linear_to_gamma(r / 255.999));
  g = uint16_t(255.999 * linear_to_gamma(g / 255.999));
  b = uint16_t(255.999 * linear_to_gamma(b / 255.999));

  // Utilize the interval class to clamp the color values
  static const interval intensity(0.000, 255.999);
  r = intensity.clamp(r);
  g = intensity.clamp(g);
  b = intensity.clamp(b);

  // Convert the RGB color to a 16-bit color
  uint16_t color = tft.color565(r, g, b);

  // Print the color to the Serial Monitor
    // Serial.print("Color: ");
    // Serial.print(r);
    // Serial.print(", ");
    // Serial.print(g);
    // Serial.print(", ");
    // Serial.println(b);
  // Write the color to the display
  tft.drawPixel(x, y, color);
}

// Define a function to write the color to the display

#endif 