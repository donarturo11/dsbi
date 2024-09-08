/*
*
*   Donarturo11 - Damn Simple Brainwave Interface
*   Firmware to send brainwaves data values to computer
*   https://github.com/donarturo11/dsbi
*
*   
*   
*/

#if defined(__AVR_ATtiny85__)
#define ANALOGPIN 1
#define BAUD 115200
#define ADC_RESOLUTION 10
#elif defined(ESP32)
#define ANALOGPIN 34
#define BAUD 115200
#define ADC_RESOLUTION 12
#else
#define ANALOGPIN A0
#define BAUD 115200
#define ADC_RESOLUTION 10
#endif

#define SAMPLE_RATE 512
#define SAMPLE_DELAY 1000000 / SAMPLE_RATE

unsigned short sample = 0;

void setup() {
  Serial.begin(BAUD);
  #ifdef AREF_MODE
  analogReference(AREF_MODE);
  #endif
}

void loop() {
  uint8_t msb_byte = 0x80;
  uint8_t lsb_byte = 0x40;  
  #if ADC_RESOLUTION == 10
  unsigned int adcTotal = 0;
  for (char i=0; i<16; i++) {
      adcTotal += analogRead(ANALOGPIN);
  }
  adcTotal = (adcTotal >> 2);
  sample = (adcTotal)&0xFFFF;
  #elif ADC_RESOLUTION == 12
  sample = analogRead(ANALOGPIN);
  #endif
  msb_byte |= (sample >> 6) & 0x3F;
  lsb_byte |= sample & 0x3F;
  Serial.write( msb_byte );
  Serial.write( lsb_byte );
  delayMicroseconds(SAMPLE_DELAY);
}
