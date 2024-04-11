#include "tx.h"
#include "fifo.h"

static fifo_t TXQUEUE;
volatile uint8_t TXDATA[BUFFER_SIZE];
struct {
    uint16_t frame;
    uint8_t busy;
} TXBUF;

void TX_sendbyte(uint8_t byte) {
    cli();
    TXBUF.frame = 1 << 9;
    TXBUF.frame |= ((uint16_t) byte << 1) ;
    TXBUF.busy = 10;
    OCR0B = 190; // 38400
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
    if (TXBUF.busy) {
        if (TXBUF.frame & 1) sbi(PORTB, TXPIN);
        else cbi(PORTB, TXPIN);
        TXBUF.busy--;
        TXBUF.frame >>= 1;
    } else if (TXQUEUE.available && !TXBUF.busy) {
        TX_sendbyte(fifo_dequeue(&TXQUEUE));
    }
    TCNT0 = 0;
    sei();
}

void TX_init() {
    TIMER_init();
    fifo_init(&TXQUEUE, TXDATA, BUFFER_SIZE);
    sbi(DDRB, TXPIN); // setup pin as output
    sbi(PORTB, TXPIN); // setup TTL as idle
}

void TX_putc(uint8_t value) {
    if(TXQUEUE.available < TXQUEUE.size)
        fifo_enqueue(&TXQUEUE, value);
    if (!TXBUF.busy) TX_sendbyte(fifo_dequeue(&TXQUEUE));
}
