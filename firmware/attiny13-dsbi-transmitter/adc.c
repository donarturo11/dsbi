#include "global.h"
#include "adc.h"

void adc_init() {
    cbi(ADMUX, MUX0);
    sbi(ADMUX, MUX1);
    sbi(ADMUX, REFS0);
    cbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADEN);
}

uint16_t adc_sample() {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

uint16_t adc_read() {
    uint32_t adc_sum = 0;
    for (uint8_t i=0; i<16; i++) {
        adc_sum += adc_sample();
    }
    return (adc_sum >> 2) & 0xFFFF;
}
