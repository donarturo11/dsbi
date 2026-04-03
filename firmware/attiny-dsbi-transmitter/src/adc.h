/* vim: set sts=4 sw=4 et: */

#ifndef ADC_H
#define ADC_H
#include "global.h"
#define SAMPLE_BITS 12
#define ADC_BITS 10
#define DECIMATION_COUNT (SAMPLE_BITS-ADC_BITS)
#define OVERSAMPLE_COUNT (1<< (DECIMATION_COUNT*2))
#define SAMPLE_CYCLES (F_CPU/(SAMPLE_RATE*OVERSAMPLE_COUNT))
#define SAMPLE_DELAY (uint8_t)( ((SAMPLE_CYCLES) / TIMER_PRESCALER) )
#include "handlers.h"
extern volatile data_handler_t ADC_handler;
register volatile uint16_t ADC_acc asm("r8");
register volatile uint16_t ADC_result asm("r10");
register volatile uint8_t ADC_cnt asm("r12");

void ADC_init();
void ADC_begin();
void ADC_end();
void ADC__timer_isr();
void ADC_poll();
void ADC__on_oversampling_ready();
void ADC__on_sampling_ready();
#endif
