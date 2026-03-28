/* vim: sts=4 sw=4 et: */

#include "buffer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

volatile buffer_t OUTPUT_BUFFER;
volatile buffer_t INPUT_BUFFER;
volatile char OUTPUT_BUFFER_DATA[BUFFER_SIZE];
volatile char INPUT_BUFFER_DATA[BUFFER_SIZE];

void buffer_init(buffer_t * buf, void * data, uint8_t size)
{
    *buf = (buffer_t){ data, size-1, 0, 0 };
}

uint8_t buffer_write(buffer_t * buf, void * data, uint8_t nmemb)
{
    uint8_t idx = 0;
    while( idx < nmemb ){
        uint8_t headtmp = (buf->head+1)&buf->mask;
        if( headtmp == buf->tail) break;
        *(buf->data + buf->head)=*((char*) data + idx);
        buf->head = headtmp;
        idx++;
    }
    nmemb=idx;
    return nmemb;
}

uint8_t buffer_read(buffer_t * buf, void * data, uint8_t nmemb)
{
    uint8_t idx = 0;
    while(idx < nmemb) {
        if (buf->tail == buf->head) break;
        *((char *) data+idx) = *((char*)(buf->data)+(buf->tail));
        buf->tail = (buf->tail+1) & buf->mask;
        idx++;
    }
    nmemb=idx;
    return nmemb;
}
