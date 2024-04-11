#ifndef ADC_H
#define ADC_H
#include "global.h"

void adc_init();
uint16_t adc_read();
uint16_t adc_oversample();
#endif
