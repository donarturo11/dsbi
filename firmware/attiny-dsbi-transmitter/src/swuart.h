/* vim: set sts=4 sw=4 et: */

#ifndef SWUART_H
#define SWUART_H
#include "swuart_config.h"
#include <avr/io.h>

register volatile uint8_t rxcnt  asm("r4");
register volatile char    rxbyte asm("r5");
register volatile uint8_t txcnt  asm("r6");
register volatile char    txbyte asm("r7");


#define TXBUSY (txcnt)
#define RXBUSY (rxcnt)
#define RXDATAREADY (rxcnt & (1<<7))

void SWUART_init();
uint8_t SWUART_put(char);
uint8_t SWUART_get();

void SWUART__txloop() __attribute__((naked));
void SWUART__txstart() ;//__attribute__((naked));
void SWUART__txstop() ;//__attribute__((naked));
void SWUART__tx_isr() __attribute__((naked));

void SWUART__rxstart();
void SWUART__rxstop();
void SWUART__rxloop() __attribute__((naked));
void SWUART__rx_isr() __attribute__((naked));
void SWUART__start_rxlisten();
void SWUART__stop_rxlisten();
#endif
