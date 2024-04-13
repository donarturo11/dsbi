#include "fifo.h"
#include <stddef.h>

void fifo_init(fifo_t *fifo, uint8_t *buffer, uint8_t size)
{
    fifo->buffer = buffer;
    fifo->size = size;
    fifo->head = 0;
    fifo->tail = 0;
    fifo->available = 0;
}

void fifo_destroy(fifo_t *fifo)
{
    fifo->buffer = NULL;
    fifo->size = 0;
    fifo->tail = 0;
    fifo->head = 0;
    fifo->available = 0;
}

void fifo_enqueue(fifo_t *fifo, uint8_t byte)
{
    fifo->buffer[fifo->tail]=byte;
    if (fifo->tail < (fifo->size)-1) {
        fifo->tail++;
        fifo->available++;
    }
    else fifo->tail = 0;
}

uint8_t fifo_dequeue(fifo_t *fifo)
{
    uint8_t value = fifo->buffer[fifo->head];
    fifo->buffer[fifo->head] = 0;
    if (fifo->head < (fifo->size)-1) {
        fifo->head++;
        fifo->available--;
    } else {
        fifo->head = 0;
    }
    return value;
}
