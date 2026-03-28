/* vim: sts=4 sw=4 et: */

#include "swuart.h"
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "swuart_config.h"
 
void SWUART_delay() __attribute__((naked));
void SWUART_delay() {
    asm volatile (
    "ldi r16,%[delay]\n"
    "1:\n"
    "dec r16\n"
    "brne 1b\n"
    "ret\n"
    :
    :[delay] "i" (BAUD_DELAY_2)
    );
}

void SWUART__txloop() {
    /* This loop can be handled by interrupt ISR */
    asm volatile (
    "sbrc %[txcnt],7\n" 
    "rjmp 4f\n"          /* if bit 7 is set, start bit will be sent */
    "lsr %[txbyte]\n\t"
    "1:\n\t"
    "brcc 2f\n\t"
    "cbi %[txport],%[txpin]\n\t"
    "rjmp 3f\n\t"
    "2:\n\t"
    "sbi %[txport],%[txpin]\n\t"
    "nop\n"
    "3:\n"
    "dec %[txcnt]\n"
    "ret\n\t"
    "4:\n"       /* send start bit  */
    "clt\n"
    "bld %[txcnt],7\n"
    "sec\n"
    "rjmp 1b\n"
    : 
    :
        [txbyte] "r" (txbyte),
        [txcnt] "r" (txcnt),
        [txport] "i" (_SFR_IO_ADDR(UART_PORT)),
        [txpin] "I" (TXPIN)
    );
}

void SWUART__txstart() {
    UART_DDR |= TXMASK;
    txcnt = 1+8+STOP_BITS;
    txcnt |= (1<<7); // set bit 7 to send a start bit
    OCR0B=(TCNT0+BAUD_DELAY)&0xFF;
    TIMSK|=(1<<OCIE0B);
}
void SWUART__txstop() {
    UART_DDR &= ~TXMASK;
    TIMSK&=~(1<<OCIE0B);
    OCR0B=0;
    txbyte=0;
    txcnt=0;
}

int SWUART_init() {
    txcnt=0;
    txbyte=0;
    rxcnt=0;
    rxbyte=0;
}

int SWUART_put(volatile char c)
{
    if (txcnt) return -1;
    txbyte = ~c;
    SWUART__txstart();
    return 0;
}

int SWUART_get()
{
    UART_DDR &= ~RXMASK;
    rxcnt = 0xFF;
    rxbyte = 0;
    wait_rx:
        if (!(UART_PIN & RXMASK))
            goto rx_init;
    if (--rxcnt) goto wait_rx;
    return -1;
    rx_init:
        rxcnt = 8;
        SWUART_delay();
    rx_loop:
        SWUART_delay();
        SWUART_delay();
        if (UART_PIN & RXMASK) rxbyte |= 0x80;
        else rxbyte &= 0x7F;
        if(--rxcnt) {
            rxbyte >>= 1;
            goto rx_loop;
        }
    return (unsigned char) rxbyte;
}                     
