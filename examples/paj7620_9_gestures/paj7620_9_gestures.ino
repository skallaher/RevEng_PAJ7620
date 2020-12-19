/*
   Author     : MarcFinns
   Modified Time: March 2017

   Author     : Aaron S. Crandall <crandall@gonzaga.edu>
   Modified Time: December 2020

   Description: This demo can recognize 9 gestures and output the result, including move up, move down, move left, move right,
   move forward, move backward, circle-clockwise, circle-counter clockwise, and wave.
*/

#include <Wire.h>

// Includes enum definition of GES_* return values from readGesture()
#include "RevEng_PAJ7620.h"

// Create gesture sensor driver object
RevEng_PAJ7620U sensor = RevEng_PAJ7620U();


// *********************************************************************
void setup()
{
  int error_code = 0;

  Serial.begin(115200);
  Serial.println("\nPAJ7620U2 sensor demo: Recognizing 9 gestures.");

  error_code = sensor.begin();			// Initialize PAJ7620 registers
                                    // return value of 0 == success
  if (error_code)
  {
    Serial.print("PAJ7620U2 init error code: ");
    Serial.println(error_code);
  }
  else
  {
    Serial.println("PAJ7620U2 init: OK");
  }

  sensor.setGestureExitTime(0);
  sensor.setGestureEntryTime(0);

  Serial.println("Please input your gestures:\n");
}


// *********************************************************************
void loop()
{
  int gesture;
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

    case GES_ANTICLOCKWISE:
      {
        Serial.print(" GES_ANTICLOCKWISE");
        break;
      }

    case GES_WAVE:
      {
        Serial.print(" GES_WAVE");
        break;
      }
    case GES_NONE:
      {
        // Serial.print(" GES_NONE");
        break;
      }
  }

  if( gesture != GES_NONE )
  {
    Serial.println(", Code: " + String(gesture));
  }

  delay(100);
}