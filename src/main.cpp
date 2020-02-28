#include <Keyboard.h>
#include <movingAvg.h>

// Original values were 200 and then 600
const int PressedMaxThreshold = 500;
const int ReleasedMinThreshold = 900;

const byte PinCount = 8;
const byte InputPins[PinCount] = {A6, A7, A8, A9, A3, A2, A1, A0};
const char KeyCodes[PinCount] = {'i', 'o', 'p', 'z', 'x', 'c', 'v', 'b'};

struct TouchInput
{
  byte analogPin;
  char keycode;
  movingAvg filter = movingAvg(20);
  boolean wasPressed = false;
};

TouchInput Pins[PinCount];

int enablePin = 5;

void setup()
{
  pinMode(enablePin, INPUT_PULLUP);
  if (digitalRead(enablePin) == HIGH)
  {
    Serial.begin(115200);
    // Keyboard.begin();

    for (int i = 0; i < PinCount; i++)
    {
      int pin = InputPins[i];

      // Set up input pins
      // de-activate the internal pull-ups, since we're using external resistors
      pinMode(pin, INPUT);

      Pins[i].analogPin = pin;
      Pins[i].keycode = KeyCodes[i];
      Pins[i].filter.begin();
    }
  }
  else
  {
    Serial.begin(9600);
    Serial.println("Keyboard init skipped.");

    pinMode(7, INPUT_PULLUP);
    pinMode(16, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);

    pinMode(10, INPUT_PULLUP);
  }
}

void loop()
{
  if (digitalRead(enablePin) == HIGH)
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
    //inputs test
    Serial.println("Keyboard init skipped.");

    Serial.print("7: ");
    Serial.println(digitalRead(7));

    Serial.print("16: ");
    Serial.println(digitalRead(16));

    Serial.print("14: ");
    Serial.println(digitalRead(14));

    Serial.print("15: ");
    Serial.println(digitalRead(15));
    
    Serial.print("10: ");
    Serial.println(analogRead(10));

    delay(1000);
  }
}
