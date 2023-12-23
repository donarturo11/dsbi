#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define BAUDRATE 33400
#define BITLENGTH (1000000/BAUDRATE) - 1
#define TXPIN PB1

void adc_init() {
    ADMUX |= (1 << MUX0);
    ADMUX |= (1 << MUX1);
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADPS2);
    ADCSRA |= (1 << ADPS1);
    ADCSRA |= (1 << ADPS0);
    ADCSRA |= (1 << ADEN);
}

int adc_read() {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ((ADCH << 2) | ADCL);
}

void TX_setup() {
    sbi(DDRB, TXPIN); // setup pin as output
    sbi(PORTB, TXPIN); // setup TTL as idle
}

void TX_putc(uint8_t value) {
    uint8_t counter = 1;
    cbi(PORTB, TXPIN); // put start bit
    _delay_us(BITLENGTH);
    if ((value & 1)) sbi(PORTB, TXPIN);
    else cbi(PORTB, TXPIN);
    _delay_us(BITLENGTH);
    do {
        if ((value >> counter) & 1) sbi(PORTB, TXPIN);
        else cbi(PORTB, TXPIN);
        _delay_us(BITLENGTH);
        counter++;
    } while (counter < 8);
    sbi(PORTB, TXPIN); // put stop bit
    _delay_us(BITLENGTH);
    _delay_us(BITLENGTH);
}

int main() {
    adc_init();
    while(1) {
        int adc = adc_read();
        TX_putc(0x80 | (adc & 0x3F));
        TX_putc(0x40 | ((adc >> 6) & 0x3F));
    }
    return 0;
}
