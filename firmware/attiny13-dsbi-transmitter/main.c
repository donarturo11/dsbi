#include "adc.h"
#include "global.h"
#include "fifo.h"

static uint32_t sampledValue = 0;
static uint8_t sample_cnt = 0;
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

// =====
void TX_setup();
void TX_putc(uint8_t value);

void testpin_init() {
    cbi(PORTB, TESTPIN);
}

void TX_setup() {
    sbi(DDRB, TXPIN); // setup pin as output
    sbi(PORTB, TXPIN); // setup TTL as idle
}

void TX_putc(uint8_t value) {
    if(TXQUEUE.available < TXQUEUE.size)
        fifo_enqueue(&TXQUEUE, value);
    if (!TXBUF.busy) TX_sendbyte(fifo_dequeue(&TXQUEUE));
}

int main() {
    cli();
    fifo_init(&TXQUEUE, TXDATA, BUFFER_SIZE);
    TX_setup();
    adc_init();
    TIMER_init();
    sei();
    uint8_t byte_msb, byte_lsb;
    while(1) {
        sampledValue += adc_read();
        if (sample_cnt == 16) {
            uint16_t adc = 0;
            byte_msb = 0x80;
            byte_lsb = 0x40;
            adc = (sampledValue / 4);
            byte_msb |= (adc >> 6) & 0x3F;
            byte_lsb |= adc & 0x3F;
            _delay_us(10);
            TX_putc( byte_msb );
            TX_putc( byte_lsb );
            _delay_us(10);
            sample_cnt = 0;
            sampledValue = 0;
        } else {
            _delay_us(0);
            sample_cnt++;
        }
    }
    return 0;
}
