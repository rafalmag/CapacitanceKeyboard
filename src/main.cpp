#include <Keyboard.h>
#include <movingAvg.h>

// A10 to adj treshold
// Original values were 200 and then 600
// const int PressedMaxThreshold = 500;
// const int ReleasedMinThreshold = 900;
const int ThresholdDiff = 150;

const byte PinCount = 8;

const byte InputPins[PinCount] = {A8, A9, A6, A7,
                                  A1, A0, A2, A3};

const char KeyCodes[PinCount] = {'z', 'x', 'c', 'v',
                                 'b', 'n', 'm', ','};

// control pins
// 7,16,14,15
#define ENABLE_PIN 15

#define LED_YELLOW 5
#define LED_GREEN 3

struct TouchInput
{
  byte analogPin;
  char keycode;
  movingAvg filter = movingAvg(20);
  boolean wasPressed = false;
};

TouchInput Pins[PinCount];

void setup()
{
  pinMode(LED_YELLOW, OUTPUT);
  digitalWrite(LED_YELLOW, LOW);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, LOW);

  pinMode(ENABLE_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  // Keyboard.begin();

  for (int i = 0; i < PinCount; i++)
  {
    int pin = InputPins[i];

    // Set up input pins, external 22M ohm pull-ups
    pinMode(pin, INPUT);

    Pins[i].analogPin = pin;
    Pins[i].keycode = KeyCodes[i];
    Pins[i].filter.begin();
  }

  pinMode(7, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);

  pinMode(10, INPUT);
}

void loop()
{
  if (digitalRead(ENABLE_PIN) == HIGH)
  {
    digitalWrite(LED_YELLOW, LOW);
    boolean anyPressed = false;
    for (int i = 0; i < PinCount; i++)
    {
      float currentAverage = Pins[i].filter.reading(analogRead(Pins[i].analogPin));
      boolean previousState = Pins[i].wasPressed;
      boolean currentState = previousState; // Default if in the dead zone

      int threshold = analogRead(10);
      int pressedMaxThreshold = max(threshold - ThresholdDiff, 1);
      int releasedMinThreshold = min(threshold + ThresholdDiff, 1023);
      if (currentAverage < pressedMaxThreshold)
        currentState = true; // Pressed
      else if (currentAverage > releasedMinThreshold)
        currentState = false; // Released

      anyPressed |= currentState;

      if (currentState != previousState)
      {
        if (currentState)
          Keyboard.press(Pins[i].keycode);
        else
          Keyboard.release(Pins[i].keycode);
      }
      Pins[i].wasPressed = currentState;
    }
    digitalWrite(LED_GREEN, anyPressed);
  }
  else
  {
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_GREEN, LOW);
    //inputs test
    Serial.println("Keyboard init skipped.");

    Serial.print("7: ");
    Serial.println(digitalRead(7));

    Serial.print("16: ");
    Serial.println(digitalRead(16));

    Serial.print("14: ");
    Serial.println(digitalRead(14));

    Serial.print("10: ");
    Serial.println(analogRead(10));

    delay(1000);
  }
}
