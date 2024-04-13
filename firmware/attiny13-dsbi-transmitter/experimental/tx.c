#include "tx.h"
#include "fifo.h"
#include <util/delay.h>

static fifo_t TXQUEUE;
static uint8_t TXDATA[BUFFER_SIZE];
static uint16_t TXFRAME;
static uint32_t TX_baudrate = 0;

void TX_sendbyte(uint8_t byte);

void TX_init(uint32_t baudrate) {
    TX_baudrate = baudrate;
    TIMER_init();
    fifo_init(&TXQUEUE, TXDATA, BUFFER_SIZE);
    sbi(DDRB, TXPIN); // setup pin as output
    sbi(PORTB, TXPIN); // setup TTL as idle
}

void TX_putc(uint8_t value) {
    sbi(PORTB, TXPIN);
    if(TXQUEUE.available < TXQUEUE.size)
        fifo_enqueue(&TXQUEUE, value);
    if (!TXFRAME) TX_sendbyte(fifo_dequeue(&TXQUEUE));
}

void TX_sendbyte(uint8_t byte) {
    sbi(PORTB, TXPIN);
    TXFRAME = 0;
    TXFRAME = 0x3 << 9;
    TXFRAME |= ((uint16_t) byte << 1) ;
    TCNT0 = 0;
    TCCR0B = (1<<CS00);
    TX_counter = 0;
}

void TX_sendbit() {
    if ( TXFRAME && (TXFRAME & 1) ) sbi(PORTB, TXPIN);
    if ( TXFRAME && !(TXFRAME & 1) ) cbi(PORTB, TXPIN);
}

void TX_shift() {
    if (TXFRAME) {
        TXFRAME >>= 1;
    } else if (!TXFRAME && TXQUEUE.available){
        sbi(PORTB, TXPIN);
        TX_sendbyte(fifo_dequeue(&TXQUEUE));
    } else if (!TXFRAME && !TXQUEUE.available) {
        TCNT0 = 0;
        TCCR0B = 0;
        TX_counter = 0;
    } else {
        sbi(PORTB, TXPIN);
    }
}

uint8_t TX_available() {
    return TXQUEUE.available;
}

void TX_flush() {
    while(TX_available()) {
        fifo_dequeue(&TXQUEUE);
    }
}

void TIMER_init() {
    TCCR0A = (1<<WGM01);
    TIMSK0 |= (1<<OCIE0A);
    OCR0A = 250;  
    TCCR0B = 0;
    sbi(TIMSK0, OCIE0B);
    sbi(TIFR0, OCF0B);
}

