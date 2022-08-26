#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
// #include <stdlib.h>

#define F_CPU 1000000UL

/* Basically we have to get resolution of 1ms, and increment a variable by an interrupt each 1ms.
    Then, if we get to the desired bpm (e.g. 60 bpm is 1000ms) we turn led on for a brief time.

 */

void timer0_init(void);

int main(void)
{
    DDRD = (1 << PD0);
    PORTD = (1 << PD0); // HUNDREDS
    
    /* test led */

    DDRB = (1 << PB0);
    // PORTB = (1 << PB0);

    /*          */

    TIMSK1 = (1 << TOIE1); // set up Overflow Interupt Enable
    TCCR1A = (1 << WGM12);  // set up Timer1 CTC. How to set the timer to generate interrupt at TOP val not MAX?
    TCCR1B = (1 << CS10) | (1 << CS11); // set up prescaler 64
    TCNT1 = 49940;

    // We choose clock source by writing to TCCR0B

    // TCCR0A = (1 << CS01) | (1 << WGM01) | (1 << WGM00); // set up prescaler and CTC
    // TCCR0B = (1 << WGM02);

    // OCR0A = 125000; // we set the value of the ticks, that trigger an overflow. <- OCR0 is timer0 (8bit)

    // Read about TCNT0 and OCR0A
    // TIMSK0 = (1 << OCIE0A);
    // TIFR0 = (1 << OCF0A);

    /* When the >TOIE0< bit is written to one (in TIMSK0), and the I-bit in the status register 
    is set, the Timer/Counter0 overflow interrupt is enabled.  */

    sei(); // Sets SREG bit 7 - I: Global Interrupt Enable
    // PORTD = 0;

    /* Common anode - turn on with a logic 0. */

    while (1)
    {
        
        // _delay_ms(1000);
    }
    
}

void timer0_init(void)
{

}

ISR(TIMER1_OVF_vect)
{
    // static volatile int8_t counter = 0;
    PORTB ^= (1 << PB0);
    TCNT1 = 49940;

    // counter++;

}
