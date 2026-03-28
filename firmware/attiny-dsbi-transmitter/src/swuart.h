/* vim: sts=4 sw=4 et: */

#ifndef SWUART_H
#define SWUART_H
#include "swuart_config.h"
#include <avr/io.h>

//register volatile uint8_t loopcnt  asm("r8");
register volatile uint8_t rxcnt  asm("r6");
register volatile uint8_t rxbyte asm("r7");
register volatile uint8_t txcnt  asm("r8");
register volatile char    txbyte  asm("r9");

//int SWUART__txloop2() __attribute__((naked));
void SWUART__txloop() __attribute__((naked));
void SWUART__txstart() ;//__attribute__((naked));
void SWUART__txstop() ;//__attribute__((naked));
int SWUART_init();
int SWUART_put(char);
int SWUART_get();
#endif
