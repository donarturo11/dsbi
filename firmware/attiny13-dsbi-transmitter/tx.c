#include "tx.h"
void TX_init() {
    sbi(DDRB, TXPIN);
    sbi(PORTB, TXPIN);
}

void TX_putc(uint8_t byte) {
    // 8-N-2
    volatile uint16_t frame = 0;
    frame |= 0x3 << 9;
    frame |= byte << 1;
    do {
        if(frame & 0x01) {
            sbi(PORTB,TXPIN);
        } else {
            cbi(PORTB,TXPIN);
        }
        _delay_us(BAUDDELAY);
        frame >>= 1;

    } while (frame);
}
