/*

   Author     : MarcFinns
   Modified Time: March 2017

   Author     : Aaron S. Crandall <crandall@gonzaga.edu>
   Modified Time: December 2020

   Description: This demo only uses the up and down gestures to control the builtin LED
*/

#include <Wire.h>

// Includes enum definition of GES_* return values from readGesture()
#include "libpaj7620.h"

// Create gesture sensor driver object
PAJ7620U sensor = PAJ7620U();


// ******************************************************************
void setup()
{
  int error_code = 0;

  pinMode(LED_BUILTIN, OUTPUT);     // Configure LED for output

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

  Serial.println("Please input your gestures:\n");
}


// ******************************************************************
void loop()
{

  int gesture;

  // Read gesture from sensor - returns a value of GES_* from GESTURES enum in libpaj7620.h
  gesture = sensor.readGesture();

  if (gesture == GES_UP)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("GES_UP");
  }
  else if(gesture == GES_DOWN)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("GES_DOWN");
  }

  delay(100);
}