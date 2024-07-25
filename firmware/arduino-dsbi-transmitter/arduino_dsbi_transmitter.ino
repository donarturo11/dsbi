// Donarturo11
// Sketch for Arduino with 10-bit adc

unsigned short sample = 0;
unsigned int adcTotal = 0;
void setup() {
  Serial.begin(19200);
  delay(1000);
}

void loop() {
  adcTotal = 0;
  adcValue = 0;
  unsigned char msb_byte = 0x80;
  unsigned char lsb_byte = 0x40;  
  // oversampling
  for (unsigned char i=0; i<16; i++) {
      adcTotal += analogRead(A0);
  }
  sample = (adcTotal >> 2) & 0xFFFF;
  msb_byte |= (sample >> 6) & 0x3F;
  lsb_byte |= sample & 0x3F;
  Serial.write( msb_byte );
  Serial.write( lsb_byte );
  delay(2);
}