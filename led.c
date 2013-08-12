#include <avr/io.h>
#include "tools.h"

void led_init( void )
{
    DDRB |= BIT(5);   // configure the pin as output
}

void led_on( void )
{
    PORTB |= BIT(5);  // drive the pin high to turn on the LED
}

void led_off( void )
{
    PORTB &= ~BIT(5);  // drive the pin low to turn off the LED
}
