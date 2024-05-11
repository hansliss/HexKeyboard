# HexKeyboard

![HexKeyboard_1](Photos/HexKeyboard_1.jpg)

The matrix rows are on pin 4, 12, 6, 7, 19 (previous version: 0, 1, 4, 30, 29)
The matrix columns are on pin 22, 21, 20, 19, 18

The SD card holder is connected to SPI, plus
Card Detect = PB0 / pin 17
Chip Enable = PB4 / pin 8
Note that CD is connected to the RXLED pin, so it will probably collide with serial data.

The display is connected like this:

* CLK = 9
* CS = 10
* DIN = 11

This requires a local max7219.h with the defines modified.

Note that pin 30 is the TXLED pin on Arduino Leonardo. Just disable interrupts while scanning
and no one will suffer.

### Serial LED pins
If you want to eliminate problems with the two LED pins (see above), you can always just disable the entire functionality, by modifying USBCore.cpp (probably found here: _C:\Users\Hans Liss\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.6\cores\arduino_).

Just comment out everything to do with RXLED and TXLED, and the file will be recompiled upon the next build.

### Errata
The SPI pins for the MAX7219 display aren't actually SPI pins. Still, the max7219 library just bit-bangs the protocol so it doesn't really matter.

The SD card holder is turned the wrong way around.

