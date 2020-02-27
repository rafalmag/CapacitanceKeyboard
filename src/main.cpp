#include <Keyboard.h>
#include <movingAvg.h>

// Original values were 200 and then 600
const int PressedMaxThreshold = 800;
const int ReleasedMinThreshold = 900;

// const byte PinCount = 6;
// const byte InputPins[PinCount] = {A0, A1, A2, A3, A4, A5};
// const char KeyCodes[PinCount] = {'d', 's', 'w', 'a', 'z', 'e'};

const byte PinCount = 1;
const byte InputPins[PinCount] = {A0};
const char KeyCodes[PinCount] = {'d'};

struct TouchInput
{
  byte analogPin;
  char keycode;
  movingAvg filter = movingAvg(20);
  boolean wasPressed = false;
};

TouchInput Pins[PinCount];

int enablePin = 5;
boolean keyboardEnabled = false;

void setup()
{
  pinMode(enablePin, INPUT);
  keyboardEnabled = digitalRead(enablePin) == HIGH;
  if (keyboardEnabled)
  {
    Serial.begin(115200);
    // Keyboard.begin();

    for (int i = 0; i < PinCount; i++)
    {
      int pin = InputPins[i];

      // Set up input pins
      // de-activate the internal pull-ups, since we're using external resistors
      pinMode(pin, INPUT);
      digitalWrite(pin, LOW);

      Pins[i].analogPin = pin;
      Pins[i].keycode = KeyCodes[i];
      Pins[i].filter.begin();
    }
  }
  else
  {
    Serial.begin(9600);
    Serial.println("Keyboard init skipped.");
  }
}

void loop()
{
  if (keyboardEnabled)
  {
    for (int i = 0; i < PinCount; i++)
    {
      float currentAverage = Pins[i].filter.reading(analogRead(Pins[i].analogPin));
      boolean previousState = Pins[i].wasPressed;
      boolean currentState = previousState; // Default if in the dead zone

      if (currentAverage < PressedMaxThreshold)
        currentState = true; // Pressed
      else if (currentAverage > ReleasedMinThreshold)
        currentState = false; // Released

      if (currentState != previousState)
      {
        if (currentState)
          Keyboard.press(Pins[i].keycode);
        else
          Keyboard.release(Pins[i].keycode);
      }
      Pins[i].wasPressed = currentState;
    }
  }
  else
  {
    Serial.println("Keyboard init skipped.");
    delay(1000);
  }
}
