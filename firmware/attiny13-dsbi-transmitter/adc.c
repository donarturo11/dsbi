#include "global.h"
#include "adc.h"

void adc_init() {
    sbi(ADMUX, MUX0);
    sbi(ADMUX, MUX1);
    sbi(ADMUX, REFS0);
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADEN);
    //sbi(ADCSRA, ADIE);
    //sbi(ADCSRA, ADIF);
}

uint16_t adc_read() {
    //sbi(PORTB, TESTPIN);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ((ADCH << 8) | ADCL);
}

uint16_t adc_oversample() {
    uint32_t ADCsum = 0;
    for (uint8_t i=0; i<16; i++) {
        ADCsum += adc_read();
    }
    return (ADCsum >> 2) & 0xFFFF;
}
