/*
   Copyright (c) 2015 seeed technology inc.
   Website    : www.seeed.cc
   Author     : Wuruibin & Xiangnan
   Modified Time: June 2015
   
   2017 - Modified by MarcFinns to encapsulate in class without global variables
   2020 - PROGMEM code adapted from Jaycar Electronics' work
   2020 - Modified by Aaron S. Crandall <crandall@gonzaga.edu>

   Version 1.2
   
   Description: This demo can recognize 9 gestures and output the result, including move up, move down, move left, move right,
  				move forward, move backward, circle-clockwise, circle-counter clockwise, and wave.

   The MIT License (MIT)

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include "Arduino.h"
#include "libpaj7620.h"


/****************************************************************
   Function Name: begin
   Description:  PAJ7620 device I2C connect and initialize
   Parameters: none
   Return: error code; success: return 0
****************************************************************/
uint8_t PAJ7620U::begin()
{
  // Reasonable timing delay values to make algorithm insensitive to
  //  hand entry and exit moves before and after detecting a gesture
  gestureEntryTime = 0;
  gestureExitTime = 200;

  Wire.begin();                       // Initialize I2C bus
  selectRegisterBank(BANK0);          // Default operations on BANK0

  if( !isPAJ7620UDevice() ) {
    return 0xFF;                      // Return error code - wrong device found
  }

  initializeDeviceSettings();         // Set all config registers

  // WARNING: Failing to select BANK0 here will make the device not work
  //  No, I don't know why - Crandall
  selectRegisterBank(BANK0);          // Gesture flag registers in Bank0

  return 0;
}


/****************************************************************
   Function Name: writeRegister
   Description:  PAJ7620 Write reg cmd
   Parameters: addr:reg address; cmd:data (byte) to write
   Return: error code; success: return 0
****************************************************************/
uint8_t PAJ7620U::writeRegister(uint8_t addr, uint8_t cmd)
{
  uint8_t result_code = 0;
  Wire.beginTransmission(PAJ7620_I2C_BUS_ADDR);   // start transmission
  Wire.write(addr);                               // send register address
  Wire.write(cmd);                                // send value to write
  result_code = Wire.endTransmission();           // end transmission
  return result_code;
}


/****************************************************************
   Function Name: readRegister
   Description:  PAJ7620 read reg data
   Parameters: addr:reg address;
           qty:number of data to read, addr continuously increase;
           data[]:storage memory start address
   Return: error code; success: return 0
****************************************************************/
uint8_t PAJ7620U::readRegister(uint8_t addr, uint8_t qty, uint8_t data[])
{
  uint8_t result_code;
  Wire.beginTransmission(PAJ7620_I2C_BUS_ADDR);
  Wire.write(addr);
  result_code = Wire.endTransmission();

  if (result_code)            //return error code - if not zero
    { return result_code; }

  Wire.requestFrom((int)PAJ7620_I2C_BUS_ADDR, (int)qty);

  while (Wire.available())
  {
    *data = Wire.read();
    data++;
  }

  return 0;
}


/****************************************************************
   Function Name: getGesturesReg0
   Description:  Read the gestures interrupt vector #0
   Parameters: &uint8_t for storing value
   Return: error code (0 means no error)
****************************************************************/
uint8_t PAJ7620U::getGesturesReg0(uint8_t data[])
  { return readRegister(PAJ7620_ADDR_GES_RESULT_0, 1, data); }


/****************************************************************
   Function Name: getGesturesReg1
   Description:  Read the gestures interrupt vector #1 (wave gesture)
   Parameters: &uint8_t for storing value
   Return: error code (0 means no error)
****************************************************************/
uint8_t PAJ7620U::getGesturesReg1(uint8_t data[])
  { return readRegister(PAJ7620_ADDR_GES_RESULT_1, 1, data); }


/****************************************************************
   Function Name: selectRegisterBank
   Description:  PAJ7620 select register bank
   Parameters: BANK0, BANK1 - see bank_e enum
   Return: none
****************************************************************/
void PAJ7620U::selectRegisterBank(bank_e bank)
{
  if( bank == BANK0 )
    { writeRegister(PAJ7620_REGISTER_BANK_SEL, PAJ7620_BANK0); }
  else if( bank == BANK1 )
    { writeRegister(PAJ7620_REGISTER_BANK_SEL, PAJ7620_BANK1); }
}


/***************************************************************
   Function Name: isPAJ7620UDevice
   Description: See if it truly a PAJ7620 at the I2C address by checking its ID
   Parameters: none
   Return: true/false
****************************************************************/
bool PAJ7620U::isPAJ7620UDevice()
{
  uint8_t data0 = 0, data1 = 0;

  // Device ID is stored in BANK0
  selectRegisterBank(BANK0);

  // Read PartID LSB[7:0] from Bank0, 0x00 - Should read 0x20
  // Read PartID MSB[15:8] from Bank0, 0x01 - Should read 0x76
  readRegister(PAJ7620_ADDR_PART_ID_0, 1, &data0);
  readRegister(PAJ7620_ADDR_PART_ID_1, 1, &data1);

  // Test if part ID is corect for PAJ7620U2
  //  See: PAJ7620U2 datasheet page 24 - 5.16 Chip/Version ID
  if ( (data0 != PAJ7620_PART_ID_LSB ) || (data1 != PAJ7620_PART_ID_MSB) )
    { return false; }

  return true;
}


/****************************************************************
   Function Name: initializeDeviceSettings
   Description: Write settings to enable I2C gesture recognition
      See header for initRegisterArray definition
   Parameters: none
   Return: none
****************************************************************/
void PAJ7620U::initializeDeviceSettings()
{
  selectRegisterBank(BANK0);  // Config starts in BANK0

  for (int i = 0; i < INIT_REG_ARRAY_SIZE; i++)
	{
    #ifdef PROGMEM_COMPATIBLE
		  uint16_t word = pgm_read_word(&initRegisterArray[i]);
    #else
      uint16_t word = initRegisterArray[i];
    #endif

		uint8_t address, value;
		address = (word & 0xFF00) >> 8;
		value = (word & 0x00FF);
		writeRegister(address, value);
  }
}


/****************************************************************
   Function Name: setGestureEntryTime
   Description: User setter for delay on gesture reads used in
      forwardBackwardGestureCheck()
   Parameters: unsigned long newGestureEntryTime
   Return: none
****************************************************************/
void PAJ7620U::setGestureEntryTime(unsigned long newGestureEntryTime)
{
  gestureEntryTime = newGestureEntryTime;
}


/****************************************************************
   Function Name: setGestureExitTime
   Description: User setter for delay after gesture reads to 
      allow the person to withdraw their hand and not cause a second
      gesture event to be thrown. 
   Parameters: unsigned long newGestureExitTime
   Return: none
****************************************************************/
void PAJ7620U::setGestureExitTime(unsigned long newGestureExitTime)
{
  gestureExitTime = newGestureExitTime;
}


/****************************************************************
   Function Name: cancelGesture
   Description: API call to clear current gesture interrupt vectors
   Parameters: none
   Return: none
****************************************************************/
void PAJ7620U::cancelGesture()
{
    uint8_t data = 0, data1 = 0;
    getGesturesReg0(&data);
    getGesturesReg1(&data1);
}


/****************************************************************
   Function Name: getWaveCount
   Description: Public API to get current count of waves by user
   Parameters: none
   Return: int quantity of waves (passes) over the sensor
****************************************************************/
int PAJ7620U::getWaveCount()
{
  uint8_t waveCount = 0;
  readRegister(PAJ7620_ADDR_WAVE_COUNT, 1, &waveCount);
  waveCount &= 0x0F;      // Count is [3:0] bits - values in 0..15
  return waveCount;
}


/****************************************************************
   Function Name: forwardBackwardGestureCheck
   Description: Used to double check a lateral gesture (up, down, left, right)
     to see if it was actually a vertical gesture (forward, backward)
   Parameters: gesture initialGesture
   Return: gesture - the double checked gesture
****************************************************************/
gesture PAJ7620U::forwardBackwardGestureCheck(gesture initialGesture)
{
  uint8_t data1 = 0;
  gesture result = initialGesture;

  delay(gestureEntryTime);
  getGesturesReg0(&data1);
  if (data1 == GES_FORWARD_FLAG)
  {
    delay(gestureExitTime);
    result = GES_FORWARD;
  }
  else if (data1 == GES_BACKWARD_FLAG)
  {
    delay(gestureExitTime);
    result = GES_BACKWARD;
  }
  return result;
}


/****************************************************************
   Function Name: readGesture
   Description: Read the latest gesture from the sensor
     -- Clears interrupt vector of gestures upon read
   Parameters: none
   Return: int (gesture enum) - gesture found or no gesture found (0)
****************************************************************/
int PAJ7620U::readGesture()
{
  uint8_t data = 0, data1 = 0, readCode = 0;
  gesture result = GES_NONE;

  readCode = getGesturesReg0(&data);
  if (readCode)
  {
    return GES_NONE;
  }
  else
  {
    switch (data)
    {
      case GES_RIGHT_FLAG:
        result = forwardBackwardGestureCheck(GES_RIGHT);
        break;

      case GES_LEFT_FLAG:
        result = forwardBackwardGestureCheck(GES_LEFT);
        break;

      case GES_UP_FLAG:
        result = forwardBackwardGestureCheck(GES_UP);
        break;

      case GES_DOWN_FLAG:
        result = forwardBackwardGestureCheck(GES_DOWN);
        break;

      case GES_FORWARD_FLAG:
        delay(gestureExitTime);
        result = GES_FORWARD;
        break;

      case GES_BACKWARD_FLAG:
        delay(gestureExitTime);
        result = GES_BACKWARD;
        break;

      case GES_CLOCKWISE_FLAG:
        result = GES_CLOCKWISE;
        break;

      case GES_ANTI_CLOCKWISE_FLAG:
        result = GES_ANTICLOCKWISE;
        break;

      default:
        getGesturesReg1(&data1);      // Reg1 (0x44) has wave flag
        if (data1 == GES_WAVE_FLAG)
          { result = GES_WAVE; }
        break;
    }
  }
  return (int)result;
}
