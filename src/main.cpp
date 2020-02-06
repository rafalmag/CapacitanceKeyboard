#include <Arduino.h>

#include "Keyboard.h"
#include "Mouse.h"

// Keyboard keyboard;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}