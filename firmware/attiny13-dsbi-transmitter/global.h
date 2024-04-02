#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#define BAUDRATE 38400
#define BITLENGTH (1000000/BAUDRATE) - 1
#define TXPIN PB1
#define BUFFER_SIZE 12
#define TESTPIN PB4
