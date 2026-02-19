MCU = atmega328p
F_CPU = 16000000
AVRARCH = avr5
FUSES = -U lfuse:w:0xff:m -U hfuse:w:0xDA:m -U efuse:w:0xFD:m -U lock:w:0xFF:m
