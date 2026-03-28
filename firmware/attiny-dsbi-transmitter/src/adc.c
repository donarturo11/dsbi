/* vim: sts=4 sw=4 et: */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "global.h"
#include "adc.h"
#include "buffer.h"

volatile data_handler_t ADC_handler;

static void ADC_nop(void* data) {
    void *d = data;
    asm("nop");
}

void ADC_init() {
    ADC_cnt=OVERSAMPLE_COUNT;
    ADC_acc = 0;
    ADC_result = 0;
    
    ADMUX|=(1<<MUX1);
    ADCSRA=(1<<ADEN)|(1<<ADIE);
    ADCSRA|=(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);
    //ADCSRA|=(1<<ADPS0);
    
    // TIMER
    OCR0A = SAMPLE_DELAY;
    
    TIMSK|=(1<<OCIE0A);
    ADC_handler.handle_data=ADC_nop;
    ADC_handler.data=0;
}



void _on_sampling_ready() {
    uint16_t result;
    ADC_cnt &= 0x7F;
    ADC_result >>= DECIMATION_COUNT;
    result = ADC_result;
    ADC_result = 0;
    ADC_handler.data=&result;
    if (ADC_handler.handle_data)
      ADC_handler.handle_data(ADC_handler.data); 
}


