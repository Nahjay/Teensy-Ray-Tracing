#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
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