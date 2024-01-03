#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <stdio.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define BAUDRATE 33400
#define BITLENGTH (1000000/BAUDRATE) - 1
#define TXPIN PB1
#define TESTPIN PB4
#define BUFFER_SIZE 12
static uint16_t sampledValue = 0;
static uint8_t sample_cnt = 0;
struct _queue_t;
typedef struct _queue_t {
    uint8_t data;
    struct *_queue_t next;
} queue_t;

static queue_t TXQUEUE;


// =======================================
struct {
    uint16_t frame;
    uint8_t busy;
} TXBUF;

void TXBUF_sendbyte(uint8_t byte) {
    if (TXBUF.busy) return;
    cli();
    TXBUF.frame = 1 << 9;
    TXBUF.frame |= ((uint16_t) byte << 1) ;
    TXBUF.busy = 10;
    OCR0B = 240;
    TCNT0 = 0;
    sei();
}

void TIMER_init() {
    TCCR0B = 0;
    TCCR0B |= 0b001;
    sbi(TIMSK0, OCIE0B);
    sbi(TIFR0, OCF0B);
    
}

ISR(TIM0_COMPB_vect) {
    cli();
    //PORTB ^= 1 << TESTPIN;
    if (TXBUF.busy) {
        sbi(PORTB, TESTPIN);
        if (TXBUF.frame & 1) sbi(PORTB, TXPIN);
        else cbi(PORTB, TXPIN);
        TXBUF.busy--;
        TXBUF.frame >>= 1;
    } else {
        cbi(PORTB, TESTPIN);
    }
    TCNT0 = 0;
    sei();
}

// =====
void TX_setup();
void TX_putc(uint8_t value);


/*
ISR(ADC_vect) {
    //if(sample_cnt == 8) sbi(PORTB, TESTPIN);
    if(sample_cnt == 16) {
        cli();
        //cbi(PORTB, TESTPIN);
        short adc = 0;
        adc = sampledValue / 16;
        BUFFER_write(0xAA);
        BUFFER_write((adc >> 8) & 0xFF);
        BUFFER_write(adc & 0xFF);
        sei();
        sample_cnt = 0;
        sampledValue = 0;
    }
    sampledValue += ((ADCH << 8) | ADCL);
    sample_cnt++;
    sbi(ADCSRA, ADSC);
}
*/
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
    //sbi(ADCSRA, ADIE);
    //sbi(ADCSRA, ADIF);
}

int adc_read() {
    sbi(PORTB, TESTPIN);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ((ADCH << 8) | ADCL);
}
/*
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
*/
void TX_setup() {
    sbi(DDRB, TXPIN); // setup pin as output
    sbi(PORTB, TXPIN); // setup TTL as idle
}

int main() {
    cli();
    //BUFFER_init();
    testpin_init();
    TX_setup();
    adc_init();
    TIMER_init();
    sei();
    //sbi(ADCSRA, ADSC);
    uint8_t data = 0;
    cbi(PORTB, TESTPIN);
    while(1) {
        sampledValue = adc_read();
        if (sample_cnt == 16) {
            short adc = 0;
            adc = sampledValue / 16;
            //BUFFER_write(0xAA);
            //BUFFER_write((adc >> 7) & 0xFF);
            //BUFFER_write(adc & 0xFF);
            _delay_us(20);
            sample_cnt = 0;
        } else {
            _delay_us(100);
            sample_cnt++;
        }
    }
    return 0;
}
