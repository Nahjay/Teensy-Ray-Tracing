#include <Arduino.h>
#include <Adafruit_ILI9341.h>

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


// put function declarations here:
int myFunction(int, int);

void setup() {
  // Set up the display by beginning the SPI connection
  SPI.setMOSI(TFT_MOSI);
  SPI.setSCK(TFT_CLK);

  // Initialize the display
  tft.begin();
}

void loop() {

  // Delay for 1 second
  delay(1000);

  // Write hello world to the display
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("Hello World!");

}

// // put function definitions here:
// int myFunction(int x, int y) {
//   return x + y;
// } 