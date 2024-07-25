//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <driver/adc.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
const int potPin = 34;
unsigned short potValue = 0;
void setup() {
  Serial.begin(38400);
  delay(1000);
}

void loop() {
  int sum=0;
  unsigned char msb_byte = 0x80;
  unsigned char lsb_byte = 0x40;
  potValue = analogRead(potPin);
  msb_byte |= (potValue >> 6) & 0x3F;
  lsb_byte |= potValue & 0x3F;
  Serial.write( msb_byte );
  Serial.write( lsb_byte );
  delay(2);
}
