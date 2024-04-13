#ifndef GLOBAL_H
#define GLOBAL_H
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <compat/deprecated.h>

// baudrate: 19200 baud length: 52us
// baudrate: 38400 baud length: 26us
#define BAUDDELAY 52
#define TXPIN PB1
#define BUFFER_SIZE 12
#define TESTPIN PB4

#endif
