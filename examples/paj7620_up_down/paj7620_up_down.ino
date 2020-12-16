/*

   Author     : MarcFinns
   Modified Time: March 2017
   Description: This demo can recognize 9 gestures and output the result, including move up, move down, move left, move right,
   move forward, move backward, circle-clockwise, circle-counter clockwise, and wave.

*/

#include <Wire.h>
#include "libpaj7620.h"

#define INTERRUPT_PIN 10

bool isr = false;
PAJ7620U sensor = PAJ7620U();

void setup()
{
  uint8_t error = 0;

  // Set interrupt pin as input
  pinMode(INTERRUPT_PIN, INPUT);

  Serial.begin(250000);
  Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

  error = sensor.begin();			// initialize Paj7620 registers
  if (error)
  {
    Serial.print("INIT ERROR,CODE:");
    Serial.println(error);
  }
  else
  {
    Serial.println("INIT OK");
  }
  Serial.println("Please input your gestures:\n");

  attachInterrupt(INTERRUPT_PIN, interruptRoutine, FALLING);
}

void loop()
{

  int gesture;
  if (isr == true)
  {
    long stamp = millis();
    isr = false;
    gesture = sensor.readGesture();

    switch (gesture) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
    {
      case GES_FORWARD:
        {
          Serial.print(" GES_FORWARD");
          break;
        }

      case GES_BACKWARD:
        {
          Serial.print(" GES_BACKWARD");
          break;
        }

      case GES_LEFT:
        {
          Serial.print(" GES_LEFT");
          break;
        }

      case GES_RIGHT:
        {
          Serial.print(" GES_RIGHT");
          break;
        }

      case GES_UP:
        {
          Serial.print(" GES_UP");
          break;
        }

      case GES_DOWN:
        {
          Serial.print(" GES_DOWN");
          break;
        }

      case GES_CLOCKWISE:
        {
          Serial.print(" GES_CLOCKWISE");
          break;
        }

      case GES_CNTRCLOCKWISE:
        {
          Serial.print(" GES_CNTRCLOCKWISE");
          break;
        }

      case GES_WAVE:
        {
          Serial.print(" GES_WAVE");
          break;
        }
      case GES_NONE:
        {
          Serial.print(" GES_NONE");
          break;
        }
    }
    Serial.println(", Code: " + String(gesture) + " - Gesture handled in " + String (millis() - stamp) + " ms");
    if (isr == true)
    {
      Serial.println(" --> Interrupt during event processing");
    }
  }
}

void interruptRoutine()
{
  isr = true;
  Serial.print("Interrupt!");
}


