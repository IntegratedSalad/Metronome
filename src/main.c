#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL
#define PRESCALER 64

static void timer1_init(void);
static void timer0_init(void);
/* take bpm as an argument and calculate that to herz */
static double calculate_herz_from_bpm(uint8_t bpm);
static uint16_t calculate_ticks_from_herz_16bit(double Hz);
volatile uint16_t start_ticks = 0; // 60 bpm
uint8_t bpm;
volatile double Hz = 0;

int main(void)
{
    DDRD = (1 << PD0);
    PORTD = (1 << PD0); // HUNDREDS

    /* test led */
    DDRB = (1 << PB0);
    /*          */

    /* 
        When the >TOIE1< bit is written to one (in TIMSK1), and the I-bit in the status register 
        is set, the Timer/Counter0 overflow interrupt is enabled.  
    */

    /* 8 bit timer, used to ring the buzzer */
    TIMSK0 = (1 << TOIE0); // Overflow Interrupt Enable
    TCCR0A = (1 << WGM01); // Set up Timer0 CTC
    //TCCR0B &= ~(1 << CS02) | ~(1 << CS01) | ~(1 << CS00); // Clear bitfield - turn off timer
   
    TIMSK1 = (1 << TOIE1); // Overflow Interupt Enable
    TCCR1A = (1 << WGM12); // set up Timer1 CTC (Clear Timer on Compare).
    TCCR1B = (1 << CS10) | (1 << CS11); // set up prescaler 64
    /*
        1000000/64/65565 = 0,2383131244 Hz
        1000000/64x = 1
        x = 15625
        65565 - 15625 = 49 940 -> number of ticks we have to start from.
    */
    bpm = 60;
    Hz = calculate_herz_from_bpm(bpm);
    start_ticks = calculate_ticks_from_herz_16bit(Hz);

    TCNT1 = start_ticks;
    TCNT0 = 0;

    sei(); // Sets SREG bit 7 - I: Global Interrupt Enable

    /* Common anode - turn on with a logic 0. */
    while (1)
    {
        /* Check if high on pins. */
    }
}

static void timer1_init(void)
{
}
static void timer0_init(void)
{
}

static double calculate_herz_from_bpm(uint8_t bpm)
{
    return bpm/60.0;
}

static uint16_t calculate_ticks_from_herz_16bit(double Hz)
{
    return UINT16_MAX - (uint16_t)round(1000000/(PRESCALER * Hz));
}

ISR(TIMER1_OVF_vect)
{
    /* When this interrupt starts, it starts another timer setting HIGH on the output pin for a brief moment, to 
       shortly blink an LED (ultimately setting off a buzzer)
     */

    PORTB ^= (1 << PB0);
    // Start Timer0
    TCCR0B = (1 << CS01); // Prescaler /8

    /* Update ticks takes bpm and returns ticks */
    TCNT1 = start_ticks; // value of 16-bit Timer1
    TCNT0 = 0;
}

ISR(TIMER0_OVF_vect)
{
    PORTB ^= (1 << PB0);
    TCCR0B &= ~(1 << CS01); // turn off timer
}