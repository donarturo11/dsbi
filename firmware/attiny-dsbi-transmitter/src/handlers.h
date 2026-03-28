/* vim: sts=4 sw=4 et: */

#ifndef HANDLERS_H
#define HANDLERS_H
typedef struct {
    void (*handle_data)(void*);
    void *data;
} data_handler_t;
typedef struct {
    uint8_t (*read)(void *, void *, uint8_t);
    uint8_t (*write)(void *, void *, uint8_t);
} IO_handler;
#endif