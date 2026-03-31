/* vim: set sts=4 sw=4 et: */

#ifndef HANDLERS_H
#define HANDLERS_H

extern volatile void (*timer0A_isr)();
extern volatile void (*timer0B_isr)();

typedef struct {
    void (*fn)(void*);
    void *data;
} data_handler_t;
typedef struct {
    uint8_t *ocr;
    const uint8_t delay;
} timer_handler_t;


extern volatile void (*timer0A_isr)();
extern volatile void (*timer0B_isr)();
extern volatile void (*pcint0_isr)();

typedef struct {
    uint8_t (*read)(void *, void *, uint8_t);
    uint8_t (*write)(void *, void *, uint8_t);
} IO_handler;
#endif