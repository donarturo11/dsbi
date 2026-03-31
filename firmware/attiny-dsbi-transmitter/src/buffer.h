/* vim: set sts=4 sw=4 et: */

#ifndef BUFFER_H
#define BUFFER_H
#include <stdint.h>
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 8
#define BUFFER_MASK BUFFER_SIZE-1
#endif

typedef struct {
  char * data;
  uint8_t mask;
  uint8_t head;
  uint8_t tail;
  
} buffer_t;

void buffer_init(buffer_t *, void * , uint8_t);
uint8_t buffer_read(buffer_t *, void *, uint8_t);
uint8_t buffer_write(buffer_t *, void *, uint8_t);

extern volatile buffer_t OUTPUT_BUFFER;
extern volatile buffer_t INPUT_BUFFER;
extern volatile char OUTPUT_BUFFER_DATA[BUFFER_SIZE];
extern volatile char INPUT_BUFFER_DATA[BUFFER_SIZE];

#endif
