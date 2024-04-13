#ifndef TX_H
#define TX_H
#include "global.h"
static uint8_t TX_counter;
void TX_init(uint32_t baudrate);
void TX_putc(uint8_t value);
void TIMER_init();
uint8_t TX_available();
void TX_sendbit();
void TX_shift();
void TX_flush();
#endif // TX_H
