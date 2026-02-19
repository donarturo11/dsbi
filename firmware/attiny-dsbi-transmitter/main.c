#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 8192
#endif

#define TIMER_PRESCALER 8
#define SAMPLE_DELAY F_CPU/TIMER_PRESCALER/SAMPLE_RATE

// SOFTWARE UART
#ifdef TXPIN
#define TXMASK (1<<TXPIN)
#endif
#ifdef RXPIN
#define RXMASK (1<<RXPIN)
#endif
#define STOP_BITS 2
#define BAUD_CYCLES ((F_CPU/BAUD_RATE))
#define BAUD_DELAY (unsigned char)( (BAUD_CYCLES/TIMER_PRESCALER)-0.5 )

volatile uint16_t ADC_sum = 0;
volatile uint8_t  ADC_cnt = 0;

ISR(TIM0_COMPA_vect) {
    uint8_t _tcnt=TCNT0;
    if(ADC_cnt<16) {
        ADMUX|=(1<<MUX1);
        ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADIE);
        ADC_cnt++;
    }
    OCR0A=(_tcnt+SAMPLE_DELAY)&0xFF;
}

ISR(ADC_vect) {
    ADC_sum+=(ADC);
}

volatile char TX_data=0xFF;
volatile char TX_status=0x10;
volatile char RX_data=0xFF;
volatile char RX_status=0x10;

#define TX_IDLE ((volatile char) (TX_status & 0x10))
#define TX_OVF  ((volatile char) (TX_status == 0x08))
#define RX_IDLE ((volatile char) (RX_status & 0x10))
#define RX_OVF  ((volatile char) (RX_status == 0x08))

ISR(TIM0_COMPB_vect) {
    // main loop
    uint8_t _tcnt=TCNT0;
    if (!TX_IDLE) {
        if(TX_data & 1 ) UART_PORT|=TXMASK;
        else  UART_PORT&=~TXMASK;
        TX_data = (TX_data >> 1) | 0x80;
        //if (TX_status == 0x08)
        if (TX_OVF)
            TX_status=0x0F-STOP_BITS;
        TX_status++;
    }
    OCR0B = (_tcnt+BAUD_DELAY)&0xFF;
}
int UART_put(char c) {
    while(!TX_IDLE) ;
    unsigned char _tcnt=TCNT0;
    cli();
    TX_data=c;
    TX_status=0x00;
    UART_PORT&=~TXMASK;
    OCR0B=(_tcnt+BAUD_DELAY)&0xFF;
    sei();
    return 0;
}

int UART_get() {
    return 0;
}

void setup();
void loop();

int main()
{
    setup();
    for (;;) {
        loop();
    }
    return 0;
}

void setup() {
    cli();
    DDRB = 0xFF;
    PORTB = 0xFF;
    OCR0A=SAMPLE_DELAY;
    OCR0B=BAUD_DELAY;
    TCCR0A=0;
    TCCR0B=(1<<CS01);
    #if defined (__AVR_ATtiny13__) || defined (__AVR_ATtiny13a__)
    #define TIMSK TIMSK0
    #endif
    TIMSK=(1<<OCIE0A)|(1<<OCIE0B);
    _delay_ms(2);
    sei();
}

void loop() {
    if (ADC_cnt>=16) {
        cli();
        short result=(ADC_sum>>2);
        ADC_sum=0;
        ADC_cnt=0;
        sei();
        UART_put(0x80 | ( (result >> 6) & 0x3F ));
        UART_put(0x40 | (  result       & 0x3F ));
    }
}