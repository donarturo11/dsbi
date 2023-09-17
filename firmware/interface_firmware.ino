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
short potValue = 0;
void setup() {
  Serial.begin(57600);
  delay(1000);
}

void loop() {
  int sum=0;
  char checksum;
  potValue = analogRead(potPin)-2048;
  Serial.write(0xAA);
  Serial.write(0xAA);
  Serial.write(0x04);
  Serial.write(0x80);
  Serial.write(0x02);
  Serial.write((potValue >> 8)&0xFF);
  Serial.write(potValue & 0xFF);
  sum += 0x80;
  sum += 0x02;
  sum += (potValue >> 8)&0xFF;
  sum += potValue&0xFF;
  checksum=~sum & 0xFF;
  Serial.write(checksum);
  delay(2);
}
