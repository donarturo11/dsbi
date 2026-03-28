/* vim: sts=4 sw=4 et: */

#ifndef UART_H
#define UART_H
#if defined(SWUART)
    #include "swuart.h"
    #ifndef UART_put
    #define UART_put SWUART_put
    #endif
    #ifndef UART_get
    #define UART_get SWUART_get
    #endif
#elif defined(AVR305)
    #include "avr305.h"
    #ifndef UART_put
    #define UART_put AVR305_put
    #endif
    #ifndef UART_get
    #define UART_get AVR305_get
    #endif
#else
    #error "This mode not implemented"
#endif
#endif
