#include "adc.h"
#include "global.h"
#include "tx.h"

void send_2byte_raw(uint16_t val) {
    uint8_t byte_msb, byte_lsb;
    byte_msb = 0x80;
    byte_lsb = 0x40;
    byte_msb |= (val >> 6) & 0x3F;
    byte_lsb |= val & 0x3F;
    TX_putc( byte_msb );
    TX_putc( byte_lsb );
}

void testpin_init() {
    cbi(PORTB, TESTPIN);
}

int main() {
    cli();
    testpin_init();
    TX_init();
    adc_init();
    sei();
    while(1) {
        sbi(PORTB, TESTPIN);
        uint16_t adc = adc_oversample();
        cbi(PORTB, TESTPIN);
        _delay_us(15);
        send_2byte_raw(adc);
        _delay_us(700);
    }
    return 0;
}
