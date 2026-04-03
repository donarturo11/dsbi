/* vim: set sts=4 sw=4 et: */

#ifndef SWUART_CONFIG_H
#define SWUART_CONFIG_H

#include "global.h"

#ifdef TXPIN
#define TXMASK (1<<TXPIN)
#endif
#ifdef RXPIN
#define RXMASK (1<<RXPIN)
#endif

#define STOP_BITS 2

#define BIT_CYCLES (F_CPU/BAUD_RATE)
#define BAUD_DELAY ((uint8_t)((BIT_CYCLES/TIMER_PRESCALER)-1))

#endif
