MCU = attiny85
F_CPU ?= 16000000
AVRARCH = avr25
FUSES = -U lfuse:w:0xE1:m -U hfuse:w:0xDF:m -U efuse:w:0xFE:m -U lock:w:0xFF:m
