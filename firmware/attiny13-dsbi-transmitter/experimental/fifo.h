#ifndef FIFO_H
#define FIFO_H
#include <inttypes.h>
typedef struct {
    uint8_t *buffer;
    uint8_t size;
    uint8_t available;
    uint8_t head;
    uint8_t tail;
} fifo_t;

void fifo_init(fifo_t *fifo, uint8_t *buffer, uint8_t size);
void fifo_destroy(fifo_t *fifo);
void fifo_enqueue(fifo_t *fifo, uint8_t byte);
uint8_t fifo_dequeue(fifo_t *fifo);

#endif // FIFO_H
