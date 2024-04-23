# dsbi
(Damn Simple Brainwave Interface)

This is simple and modular interface for EEG acquisition.
Analog part is brought from [BioAmpExg](https://github.com/upsidedownlabs/BioAmp-EXG-Pill).
The goal of this project is easy assemblation by everyone with very small experience in electronics.
Circuit was tested with ESP32 and AVR Attiny13.

# Data format
Data are sent as raw unsigned 12-bit integer wave with sample rate 512 Hz in following format:
```
unsigned short meteredValue = HHHH HHLL LLLL
unsigned char firstByte  = 10HH HHHH # ( 0x80 | ( (meteredValue >> 6) & 0x3F) )
unsigned char secondByte = 01LL LLLL # ( 0x40 |   (meteredValue & 0x3F) )
```
# Specs
## Common:
* Interface: UART
* Sample rate 512 Hz
* Raw wave 12-bit unsigned short
## ESP32
* Sampling in 12-bit without oversampling
* Hardware UART 8-N-1
* Baudrate: 38400
## Attiny13
* Sampling in 10 bit with oversampling
* Software UART 8-N-2
* Baudrate: 19200
# Compatible software
* [BrainwaveOSC](https://github.com/trentbrooks/BrainWaveOSC)
* [QBrainwaveOSC](https://github.com/donarturo11/QBrainwaveOSC)
* [libThinkGearCpp](https://github.com/donarturo11/libThinkGearCpp)
