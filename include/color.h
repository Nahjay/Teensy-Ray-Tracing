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
  // Apply gamma correction
  // double r = linear_to_gamma(pixelColor.x());
  // double g = linear_to_gamma(pixelColor.y());
  // double b = linear_to_gamma(pixelColor.z());

  double r = pixelColor.x();
  double g = pixelColor.y();
  double b = pixelColor.z();

  // Utilize the interval class to clamp the color values
  static const interval intensity(0.000, 1.000);
  r = intensity.clamp(r);
  g = intensity.clamp(g);
  b = intensity.clamp(b);

  // Convert the color to 16-bit values
  uint16_t r16 = uint16_t(255.999 * r);
  uint16_t g16 = uint16_t(255.999 * g);
  uint16_t b16 = uint16_t(255.999 * b);

  // Convert the RGB color to a 16-bit color
  uint16_t color = tft.color565(r16, g16, b16);

  // Tiny delay
  delay(1);

  // Write the color to the display
  tft.drawPixel(x, y, color);
}

#endif 