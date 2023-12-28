#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define BAUDRATE 33400
#define BITLENGTH (1000000/BAUDRATE) - 1
#define TXPIN PB1
#define TESTPIN PB4

static uint16_t sampledValue = 0;
static uint8_t sample_cnt = 0;

void TX_setup();
void TX_putc(uint8_t value);

ISR(ADC_vect) {
    if(sample_cnt == 8) sbi(PORTB, TESTPIN);
    if(sample_cnt == 16) {
        cli();
        sbi(ADCSRA, ADSC);
        TX_putc(0xAA);
        cbi(PORTB, TESTPIN);
        short adc = 0;
        adc = sampledValue / 16;
        sei();
        TX_putc((adc >> 8) & 0x0F);
        TX_putc(adc & 0xFF);
        sample_cnt = 0;
        sampledValue = 0;
    }
    sampledValue += ((ADCH << 8) | ADCL);
    sample_cnt++;
    sbi(ADCSRA, ADSC);
}

void testpin_init() {
    cbi(PORTB, TESTPIN);
}

void adc_init() {
    sbi(ADMUX, MUX0);
    sbi(ADMUX, MUX1);
    sbi(ADMUX, REFS0);
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
    sbi(ADCSRA, ADEN);
    sbi(ADCSRA, ADIE);
    sbi(ADCSRA, ADIF);
}

int adc_read() {
    sbi(PORTB, TESTPIN);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    //cbi(PORTB, TESTPIN);
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
    cli();
    testpin_init();
    TX_setup();
    adc_init();
    sei();
    sbi(ADCSRA, ADSC);
    while(1) {

    }
    return 0;
}
