/*
   Copyright (c) 2015 seeed technology inc.
   Website    : www.seeed.cc
   Author     : Wuruibin
   Modified Time: June 2015
   
   2017 - Modified by MarcFinns to encapsulate in class without global variables
   
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

#include "libpaj7620.h"
#include "Arduino.h"


/****************************************************************
   Function Name: writeRegister
   Description:  PAJ7620 Write reg cmd
   Parameters: addr:reg address; cmd:function data
   Return: error code; success: return 0
****************************************************************/
uint8_t PAJ7620U::writeRegister(uint8_t addr, uint8_t cmd)
{
  uint8_t result = 0;
  Wire.beginTransmission(PAJ7620_I2C_BUS_ADDR);   // start transmission to device
  Wire.write(addr);                     // send register address
  Wire.write(cmd);                      // send value to write
  result = Wire.endTransmission();      // end transmission
  return result;
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
  uint8_t error;
  Wire.beginTransmission(PAJ7620_I2C_BUS_ADDR);
  Wire.write(addr);
  error = Wire.endTransmission();

  if (error)
  {
    return error; //return error code
  }

  Wire.requestFrom((int)PAJ7620_I2C_BUS_ADDR, (int)qty);

  while (Wire.available())
  {
    *data = Wire.read();
    data++;
  }

  return 0;
}


/****************************************************************
****************************************************************/
uint8_t PAJ7620U::getGesturesReg0(uint8_t data[])
  { return readRegister(PAJ7620_ADDR_GES_RESULT_0, 1, data); }

uint8_t PAJ7620U::getGesturesReg1(uint8_t data[])
  { return readRegister(PAJ7620_ADDR_GES_RESULT_1, 1, data); }


/****************************************************************
   Function Name: selectRegisterBank
   Description:  PAJ7620 select register bank
   Parameters: BANK0, BANK1
   Return: none
****************************************************************/
void PAJ7620U::selectRegisterBank(bank_e bank)
{
  switch (bank) {
    case BANK0:
      writeRegister(PAJ7620_REGISTER_BANK_SEL, PAJ7620_BANK0);
      break;
    case BANK1:
      writeRegister(PAJ7620_REGISTER_BANK_SEL, PAJ7620_BANK1);
      break;
    default:
      break;
  }
}


/***************************************************************
****************************************************************/
bool PAJ7620U::isPAJ7620UDevice()
{
  uint8_t data0 = 0, data1 = 0;

  // Device ID is stored in BANK0
  selectRegisterBank(BANK0);

  // Read PartID LSB[7:0] from Bank0, 0x00 - Should read 0x20
  // Read PartID MSB[15:8] from Bank0, 0x01 - Should read 0x76
  readRegister(0, 1, &data0);
  readRegister(1, 1, &data1);

  // Test if part ID is corect for PAJ7620U2
  //  See: PAJ7620U2 datasheet page 24 - 5.16 Chip/Version ID
  if ( (data0 != 0x20 ) || (data1 != 0x76) )
    { return false; }

  return true;
}


/****************************************************************
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
****************************************************************/
void PAJ7620U::setGestureEntryTime(unsigned long newGestureEntryTime)
{
  gestureEntryTime = newGestureEntryTime;
}

/****************************************************************
****************************************************************/
void PAJ7620U::setGestureExitTime(unsigned long newGestureExitTime)
{
  gestureExitTime = newGestureExitTime;
}

/****************************************************************
****************************************************************/
void PAJ7620U::cancelGesture()
{
    // Read register to clear spurious interrupts
    uint8_t data = 0, data1 = 0;
    readRegister(PAJ7620_ADDR_GES_RESULT_0, 1, &data);
    readRegister(PAJ7620_ADDR_GES_RESULT_1, 1, &data1);
}


/****************************************************************
****************************************************************/
int PAJ7620U::getWaveCount()
{
  uint8_t waveCount = 0;
  readRegister(PAJ7620_ADDR_WAVE_COUNT, 1, &waveCount);
  waveCount &= 0x0F;      // Count is [3:0] bits - values in 0..15
  return waveCount;
}


/****************************************************************
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
****************************************************************/
int PAJ7620U::readGesture()
{
  uint8_t data = 0, data1 = 0, error = 0;
  gesture result = GES_NONE;

  error = getGesturesReg0(&data);
  if (error)
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

      case GES_COUNT_CLOCKWISE_FLAG:
        result = GES_CNTRCLOCKWISE;
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
