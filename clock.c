#include <avr/io.h>
#include <avr/interrupt.h>
#include "thread.h"

void TIMER0_init( void )
{
    TCCR0A = 0;
    TIMSK0  = 1 << TOIE0; // overflow interrupt 
    
    TCCR0B = (1<<CS02) | (1<<CS00); // /1024 scaling, and begin

}

int on = 0;

ISR( TIMER0_OVF_vect ) 
{   
    thread_yield();
}
