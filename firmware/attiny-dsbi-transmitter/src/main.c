/* vim: sts=4 sw=4 et: */

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
//#include <avr/pgmspace.h>
#include "global.h"
#include "swuart.h"
#include "buffer.h"
#include "adc.h"

void build_report(void * data)
{
    uint16_t * rep = (uint16_t *) data;
    *rep = (( *rep & 0x0FC0 ) >> 6) | (( *rep & 0x03F ) << 8) | 0x4080;
    buffer_write(&OUTPUT_BUFFER, data, 2);
}

ISR(TIM0_COMPA_vect, ISR_NAKED) {
    OCR0A=(TCNT0+SAMPLE_DELAY)&0xFF;
    ADCSRA|=(1<<ADSC);
    reti();
}

ISR(TIM0_COMPB_vect, ISR_NAKED) {
    if (txcnt) {
      OCR0B=TCNT0+BAUD_DELAY;
      SWUART__txloop();
    } else {
      SWUART__txstop();  
    }    
    reti();
}

ISR(ADC_vect, ISR_NAKED) {
    if (ADC_cnt--) {
        ADC_acc += ADC;
    } 
    if (!ADC_cnt) {
        ADC_result = ADC_acc;
        ADC_cnt=OVERSAMPLE_COUNT | 0x80;
        ADC_acc=0;
    }
    reti();
}

void setup();
void loop(); // __attribute__((naked));
int main()
{

    setup();
    for(;;)
    {
        loop();
    }
    return 0;
}

void setup()
{
    PORTB = 0xFF;
    DDRB = 0xFF;
    TCCR0A=0;
    TCCR0B=0;
    TCNT0=0;
    TCCR0B=(1<<CS01);
    
    buffer_init(&INPUT_BUFFER, (void*) INPUT_BUFFER_DATA, BUFFER_SIZE);
    buffer_init(&OUTPUT_BUFFER, (void*) OUTPUT_BUFFER_DATA, BUFFER_SIZE);
    
    SWUART_init();
    ADC_init();
    ADC_handler.handle_data=build_report;
    sei();
    TCCR0B=(1<<CS01);
}

void loop()
{
    cli();
    if (ADC_cnt & 0x80) {
        _on_sampling_ready();
    }
    if (!txcnt) {
        char byte=0x00;
        if( buffer_read(&OUTPUT_BUFFER, &byte, 1) ) {
          txbyte = ~byte;
          SWUART__txstart();
        }
    }
    sei();    
}

