# Gesture Sensor - PAJ7620

**Description**

The library here is for the PAJ7620U2 sensor with an I2C interface.
The device provides simple motion-based gesture recognition.
It can recognize 9 hand moving gestures including:
- move up
- move down
- move left
- move right
- circle clockwise
- circle anti-clockwise (counter-clockwise / widdershins)
- forwards / towards
- backards / away
- wave

The device also counts the quantity of hand waves, which is now available via an API call in version 1.2.

The sensor's datasheet is available [here](https://datasheetspdf.com/pdf-file/1309990/PixArt/PAJ7620U2/1).

The PAJ7620U2 is available from various vendors such as Seeed Studio on a breakout board.
For more information about their product, please visit the [wiki](http://wiki.seeedstudio.com/Grove-Gesture_v1.0/).
Breakout boards are also sold under the part number GY-PAJ7620 by various unnamed manufacturers.


**Installation**

To download. click the DOWNLOAD ZIP button, rename the uncompressed folder Gesture_PAJ7620. Check that the Gesture_PAJ7620 folder contains libpaj7620.cpp and libpaj7620.h

Place the Gesture_PAJ7620 library folder your arduinosketchfolder/libraries/ folder. You may need to create the libraries subfolder if its your first library. Restart the IDE.

There's a great tutorial on Arduino library installation at: http://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use

**Library History**

#### Version 1.2.0 ####
- Work done by [Aaron S. Crandall](https://github.com/acrandal) \<crandall@gonzaga.edu>.  
- Library is able to return a current count of waves by the user.  
- Library now provides several extra example Arduino programs to showcase using the wave feature and more of the API.  
- Incorporated PROGMEM based approach to storing the extensive initialization configuration adapted from the [Jaycar Electronics](https://www.jaycar.com.au/) fork, saving 21% of SRAM memory on an Arduino Uno (ATMega328P) board.  
- Lots of object refactoring and method layout to make it easier to read and test.
- Plenty of small formatting, commenting updates, and other details.  
- Bugfix: Brought interrupt bits into alignment with official documentation for gesture directions (see v0.8 of documentation).

#### Version 1.1.0 ####
- Work done by [Marc Finns](https://github.com/MarcFinns).  
- Based on the original SEEED library but with higher level API and fully encapsulated in a C++ object (no global variables).   
- No need for registry manipulation in user code anymore.  
- A gesture event is returned with an API call.  
- Library provides an interrupt based example using the PAJ7620 object.  

#### Version 1.0.0 ####
- [SEEED](https://www.seeedstudio.com/) released [library](https://github.com/Seeed-Studio/Gesture_PAJ7620) is able to identify and return all 9 gestures and up to 15 calculated gestures in example Arduino code.


**Contributors**
- Original Code from SEEED - Jack Wu, Wuruibin, & Xiangnan, June 2015
- Initial class-based solution - Marc Finns, 2017
- PROGMEM adapted from Jaycar Elctronics' fork, December 2020
- Overhaul, documentation, and current version by Aaron S. Crandall, December 2020

**License**  
MIT - see LICENSE file

**Version**  
1.2.0