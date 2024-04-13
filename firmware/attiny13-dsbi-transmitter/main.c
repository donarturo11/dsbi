#include "adc.h"
#include "global.h"
#include "tx.h"

/************************************
   (1) B5    Vcc
       B3    B2
in --> B4    B1 ---> TX 19200 bauds
       GND   B0

************************************/

void send_2byte_raw(uint16_t val) {
    uint8_t byte_msb, byte_lsb;
    byte_msb = 0x80;
    byte_lsb = 0x40;
    byte_msb |= (val >> 6) & 0x3F;
    byte_lsb |= val & 0x3F;
    TX_putc( byte_msb );
    TX_putc( byte_lsb );
}
static uint16_t adc_value = 0;
int main() {
    cli();
    TX_init();
    adc_init();
    _delay_ms(1000);
    sei();
    while(1) {
        adc_value = adc_read();
        send_2byte_raw(adc_value);
        _delay_us(607);
    }
    return 0;
}
