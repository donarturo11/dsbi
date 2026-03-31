/* vim: set sts=4 sw=4 et: */

#include "swuart.h"
#include "global.h"
#include "handlers.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "swuart_config.h"
void SWUART__txloop() {
    /* This loop can be handled by interrupt ISR */
    asm volatile (
    "sbrc %[txcnt],7\n\t"
    "rjmp 4f\n\t"          /* if bit 7 is set, start bit will be sent */
    "lsr %[txbyte]\n"
    "1:\n\t"
    "brcc 2f\n\t"
    "cbi %[txport],%[txpin]\n\t"
    "rjmp 3f\n"
    "2:\n\t"
    "sbi %[txport],%[txpin]\n\t"
    "nop\n\t"
    "3:\n\t"
    "dec %[txcnt]\n\t"
    "ret\n"
    "; send start bit\n"
    "4:\n\t"       /* send start bit  */
    "clt\n\t"
    "bld %[txcnt],7\n\t"
    "sec\n\t"
    "rjmp 1b\n\t"
    :
    :
        [txbyte] "r" (txbyte),
        [txcnt] "r" (txcnt),
        [txport] "i" (_SFR_IO_ADDR(UART_PORT)),
        [txpin] "I" (TXPIN)
    );
}

void SWUART__txstart() {
    cli();
    UART_DDR |= TXMASK;
    txcnt = 1+8+STOP_BITS;
    txcnt |= (1<<7); // set bit 7 to send a start bit
    timer0B_isr = SWUART__tx_isr;
    asm(
        "in r3,%[tcnt]\n\t"
        "ldi r25,%[delay]\n\t"
        "out %[ocr],r3\n\t"
       ::
         [ocr] "i" _SFR_IO_ADDR(OCR0B),
         [tcnt] "i" _SFR_IO_ADDR(TCNT0),
         [delay] "i" ((BAUD_DELAY))
       );
    TIMSK|=(1<<OCIE0B);
}
void SWUART__txstop() {
    UART_DDR &= ~TXMASK;
    TIMSK&=~(1<<OCIE0B);
    OCR0B=0;
    txbyte=0;
    txcnt=0;
}

void SWUART_init() {
    txcnt=0;
    txbyte=0;
    rxcnt=0;
    rxbyte=0;
}

void SWUART__tx_isr() {
    if (txcnt) {
      SWUART__txloop();
    } else {
      SWUART__txstop();
    }
    asm(
         "ldi r25,%[delay]\n\t"
         "add r3,r25\n\t"
          :: [delay] "i" (BAUD_DELAY));
    asm("ret\n");
}

void SWUART__start_rxlisten() {
    UART_DDR &= ~RXMASK;
    PCMSK |= (1<<RXPIN);
    GIMSK |= 1<<(PCIE);
    pcint0_isr = SWUART__rxstart;
}
void SWUART__stop_rxlisten() {
    PCMSK = 0;
    GIMSK &= ~(1<<PCIE);
    pcint0_isr = 0;
}

volatile void (*prevISR)();
void SWUART__rxstart()
{
    if (UART_PIN & RXMASK) return;
    asm(
        "ldi r25,%[delay]-4\n\t"
        "add r2,r25\n\t"
        "out %[ocr],r2\n\t"
        ::
    [delay] "i" (((uint8_t)BAUD_DELAY/2)),
    [ocr] "i" (_SFR_IO_ADDR(OCR0A))
    );
    SWUART__stop_rxlisten();
    rxcnt = 9;
    rxbyte = 0;
    prevISR = timer0A_isr;
    timer0A_isr = SWUART__rx_isr;
}

void SWUART__rxstop()
{
    rxcnt = 0x80;
    OCR0A = 0;
    timer0A_isr = prevISR;
    SWUART__start_rxlisten();
}

void SWUART__rxloop()
{
    asm volatile (
    "clc\n\t"
    "sbic %[rxport],%[rxpin]\n\t"
    "sec\n\t"
    "dec %[rxcnt]\n\t"
    "ror %[rxbyte]\n\t"
    "ret\n\t"
    :
    :
        [rxbyte] "r" (rxbyte),
        [rxcnt] "r" (rxcnt),
        [rxport] "i" (_SFR_IO_ADDR(UART_PIN)),
        [rxpin] "I" (RXPIN)
    );
}
void SWUART__rx_isr()
{
    if (rxcnt) {
      SWUART__rxloop();
    } else {
      SWUART__rxstop();
    }
    asm("ldi r25,%[delay]\n\t"
        "add r2,r25\n\t"
       :: [delay] "i" (BAUD_DELAY));
    asm("ret\n\t");
}


uint8_t SWUART_put(volatile char c)
{
    if (txcnt) return 1;
    txbyte = ~c;
    SWUART__txstart();
    return 0;
}

uint8_t SWUART_get()
{
    if (!(rxcnt & 0x80)) return 0;
    rxcnt &= ~(1<<7);
    return rxbyte;
}
