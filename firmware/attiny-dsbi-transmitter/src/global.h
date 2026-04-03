/* vim: set sts=4 sw=4 et: */

#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef TIMSK
#define TIMSK TIMSK0
#endif
#ifndef TIMER_PRESCALER
#define TIMER_PRESCALER 8
#endif
#ifndef SAMPLE_RATE
#define SAMPLE_RATE 512
#endif
#ifndef BAUD_RATE
#define BAUD_RATE 19200
#endif

#endif
