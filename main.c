#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "thread.h"
#include "serial.h"
#include "led.h"

struct thread this_thread;

void panic( void );

void b( void )
{

    for (;;) {
        for (int i = 0;i<20;i++) {
            printf("b");
        }
    }
}

int a( void )
{

    for (;;) {
        for (int i=0;i<20;i++) {
            printf("a");
        }
    
        return 1234;
    }
    return 0;
}

int main( void )
{
    led_init();

    USART0_init();
    printf("printf initialized\n\r");

    sei();

    threads_init(&this_thread);
    
    
    struct thread that_thread;
    thread_create(&that_thread, (void(*)(void))a);   
    
    struct thread other_thread;
    thread_create(&other_thread, b);
    

    for(;;) {
        printf("c");
    }

}

void panic( void )
{
    printf("\n\rPANIC\n\r");
    for(;;);
}
