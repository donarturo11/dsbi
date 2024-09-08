# DSBI transmitter firmware for any Arduino
Just open sketch with ArduinoIDE, build then upload.

## Instructions for Arduino IDE
This sketch was tested on Arduino Uno and Nano, Attiny85 end ESP32 WROOM.
For use with Attiny85, you must to install library ATTinyCore by Spence Konde,
then use for upload dedicated programmer like USBIISP or (if your PC has RS232 port)
use [ponyprog](https://www.lancos.com/siprogsch.html). Very good custom is install bootloader.
I recommend to upload [Optiboot](https://github.com/Optiboot/optiboot) for easy upgrade firmware
without using dedicated programmer, just UART adapter.

# Data format
Data are sent as raw unsigned 12-bit integer wave with sample rate 512 Hz in following format:
```
unsigned short meteredValue = 0bHHHHHHLLLLLL
unsigned char firstByte  = 0b10HHHHHH # ( 0x80 | ( (meteredValue >> 6) & 0x3F) )
unsigned char secondByte = 0b01LLLLLL # ( 0x40 |   (meteredValue & 0x3F) )
```

# Recommended software
Data can be parsed using [QBrainwaveOSC](https://github.com/donarturo11/QBrainwaveOSC).
For using this device, choose baudrate 115200 and Device Type as 2-Byte raw wave parser.
