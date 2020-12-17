# Gesture Sensor - PAJ7620

**Description**

The library here is for the PAJ7620U2 sensor with an I2C interface.
The device provides gesture recognition.
It can recognize 9 hand moving gestures including:
- move up
- move down
- move left
- move right
- circle clockwise
- circle counter-clockwise (widdershins)
- towards / fowards
- away / backwards
- wave

The device datasheet is available [here](https://datasheetspdf.com/pdf-file/1309990/PixArt/PAJ7620U2/1).

The PAJ7620U2 is available from various vendors such as Seeed Studio on a breakout board.
For more information about their product, please visit the [wiki](http://wiki.seeedstudio.com/Grove-Gesture_v1.0/).

Breakout boards are also sold under the part number GY-PAJ7620 by various unnamed manufacturers.

**Library History**

##### Version 1.2 #####
Work done by [Aaron S. Crandall](https://github.com/acrandal) \<crandall@gonzaga.edu>.
Library is able to return a current count of waves by the user.
Library now provides several extra example Arduino programs to showcase using the wave feature and more of the API.
Incorporated PROGMEM based approach to storing the extensive initialization configuration adapted from the [Jaycar Electronics](https://www.jaycar.com.au/) fork, saving 21% of SRAM memory on an Arduino Uno (ATMega328P) board.
Lots of object refactoring and method layout to make it easier to read and test.
Plenty of small formatting, commenting updates, and other details.

##### Version 1.1 #####
Work done by [Marc Finns](https://github.com/MarcFinns).
Based on the original SEEED library but with higher level API and fully encapsulated in a C++ object (no global variables). 
No need for registry manipulation in user code anymore.
A gesture event is returned with an API call.
Library provides an interrupt based example using the PAJ7620 object.

##### Version 1.0 #####
[SEEED](https://www.seeedstudio.com/) released [library](https://github.com/Seeed-Studio/Gesture_PAJ7620) is able to identify and return all 9 gestures and up to 15 calculated gestures in example Arduino code.


**Contributors**
- Original Code from SEEED - Jack Wu, Wuruibin, & Xiangnan, June 2015
- Initial class-based solution - Marc Finns, 2017
- PROGMEM adapted from Jaycar Elctronics' fork, December 2020
- Overhaul, documentation, and current version by Aaron S. Crandall, December 2020

**License**
MIT - see LICENSE file

**Version**
1.2