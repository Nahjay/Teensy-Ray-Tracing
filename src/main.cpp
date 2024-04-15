#include <Arduino.h>
#include <Adafruit_ILI9341.h>

// Define the pins used for the display
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_CLK 13
#define TFT_MISO 12
#define TFT_SCLK 13
#define TFT_RST 255 // Reset pin not used

// Create an instance of the display
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST, TFT_MOSI, TFT_CLK, TFT_MISO);


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  // int result = myFunction(2, 3);


}

void loop() {
  // put your main code here, to run repeatedly:
  // Print the result to the serial monitor

  Serial.begin(9600);
  Serial.println("Hello World!");
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}