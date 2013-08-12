#include <avr/io.h>
#include <stdio.h>
#include "serial.h"

static FILE uartout = FDEV_SETUP_STREAM(USART0_tx, NULL, _FDEV_SETUP_WRITE);
static FILE uartin =  FDEV_SETUP_STREAM(USART0_rx, NULL, _FDEV_SETUP_READ);

void USART0_init( void )
{

    UBRR0H = (MYUBRR >> 8) & 0xf;
    UBRR0L = MYUBRR & 0xff;
    
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    
    stdout = &uartout;
    stdin  = &uartin;
}

void USART0_tx( unsigned char data )
{
    while ( ! (UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

unsigned char USART0_rx( void )
{
    while ( ! (UCSR0A & (1<<RXC0)));
    return UDR0;
}


