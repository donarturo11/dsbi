/* vim: set sts=4 sw=4 et: */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "global.h"
#include "uart.h"
#include "buffer.h"
#include "adc.h"

void build_report(void * data)
{
    uint16_t * rep = (uint16_t *) data;
    *rep = (( *rep & 0x0FC0 ) >> 6) | (( *rep & 0x03F ) << 8) | 0x4080;
    buffer_write(&OUTPUT_BUFFER, data, 2);
}
volatile void (*timer0A_isr)();
volatile void (*timer0B_isr)();
volatile void (*pcint0_isr)();

ISR(TIM0_COMPA_vect, ISR_NAKED) {
    asm("in r2,%[tcnt]\n;\tdec r2":: [tcnt] "i" _SFR_IO_ADDR(TCNT0));
    if (timer0A_isr) timer0A_isr();
    asm(
        "out %[ocr],r2\n"
      ::
      [ocr] "i" _SFR_IO_ADDR(OCR0A)
    );
    reti();
}

ISR(TIM0_COMPB_vect, ISR_NAKED) {
    asm("in r3,%[tcnt]\n;\tdec r3":: [tcnt] "i" _SFR_IO_ADDR(TCNT0));
    if (timer0B_isr) timer0B_isr();
    asm(
      "out %[ocr],r3\n"
      ::
      [ocr] "i" _SFR_IO_ADDR(OCR0B)
    );
    reti();
}

ISR(PCINT0_vect, ISR_NAKED)
{
    asm("in r2,%[tcnt]\n;\tdec r2"
        :: [tcnt] "i" _SFR_IO_ADDR(TCNT0));
    if (pcint0_isr) pcint0_isr();
    reti();
}

ISR(ADC_vect, ISR_NAKED) {
    ADC__on_sampling_ready();
    reti();
}


void setup();
void loop();
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
    cli();
    PORTB = 0xFF;
    DDRB = 0xFF;
    buffer_init(&INPUT_BUFFER, (void*) INPUT_BUFFER_DATA, BUFFER_SIZE);
    buffer_init(&OUTPUT_BUFFER, (void*) OUTPUT_BUFFER_DATA, BUFFER_SIZE);
    SWUART_init();
    SWUART__start_rxlisten();
    ADC_init();
    ADC_handler.fn=build_report;
    ADC_begin();
    sei();
    TCCR0B=(1<<CS01);
}

void loop()
{
    cli();
    ADC_poll();
    if (!TXBUSY) {
        char byte=0x00;
        if( buffer_read(&OUTPUT_BUFFER, &byte, 1) ) {
          UART_put(byte);
        }
    }
    if (RXDATAREADY) {
        char c=UART_get();
        buffer_write(&INPUT_BUFFER, &c, 1);
    }
    {
        char data[BUFFER_SIZE];
        uint8_t n=buffer_read(&INPUT_BUFFER, data, BUFFER_SIZE);
        buffer_write(&OUTPUT_BUFFER, data, n);
    }
    sei();
}

