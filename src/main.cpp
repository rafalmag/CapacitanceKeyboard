#include <Arduino.h>

#include "Keyboard.h"
#include "Mouse.h"

// Keyboard keyboard;
int value = 0;

void setup()
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  // Keyboard.begin();
  // Mouse.begin();

  /* Set up input pins 
   DEactivate the internal pull-ups, since we're using external resistors */
  pinMode(A0, INPUT);
  digitalWrite(A0, LOW);
}

boolean low = false;
boolean logged = false;

void loop()
{
  value = analogRead(A0);
  if (value < 1000)
  {
    low = true;
    if (!logged)
    {
      Serial.print("Val A0: ");
      Serial.println(value);
      Serial.print("Volt : ");
      Serial.println(map(value, 0, 1023, 0, 5));
      Serial.println("---");
      logged = true;
    }
  }
  else
  {
    low = false;
    logged = false;
  }
  delay(10);
}