/* vim: set sts=4 sw=4 et: */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"
#include "adc.h"

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
    ADC_handler.fn=ADC_nop;
    ADC_handler.data=0;
}

void ADC_begin()
{
    OCR0A = SAMPLE_DELAY;
    TIMSK|=(1<<OCIE0A);
    timer0A_isr = ADC__timer_isr;
}

void ADC_end()
{
    OCR0A = 0;
    TIMSK&=~(1<<OCIE0A);
    timer0A_isr = 0;
}

void ADC_poll()
{
    if (ADC_cnt & 0x80) {
        ADC__on_oversampling_ready();
    }
}

void ADC__on_oversampling_ready() {
    uint16_t result;
    ADC_cnt &= 0x7F;
    ADC_result >>= DECIMATION_COUNT;
    ADC_result /= ADC_AVERAGING;
    result = (uint16_t) ADC_result;
    ADC_result = 0;
    ADC_handler.data=&result;
    if (ADC_handler.fn)
      ADC_handler.fn(ADC_handler.data);
}


void ADC__on_sampling_ready() {
    if (ADC_cnt--) {
        ADC_acc += (uint32_t) ADC;
    }
    if (!ADC_cnt) {
        ADC_result = ADC_acc;
        ADC_cnt=OVERSAMPLE_COUNT | 0x80;
        ADC_acc=0;
    }
}

void ADC__timer_isr() {
    ADCSRA|=(1<<ADSC);
    asm ("ldi r23,%[delay]\n\t"
         "add r24,r23\n\t"
         ::[delay] "i" (SAMPLE_DELAY));
}
