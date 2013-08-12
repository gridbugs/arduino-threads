#define BAUD 9600
#define FOSC 16000000
#define MYUBRR FOSC/16/BAUD-1

void USART0_init( void );
void USART0_tx( unsigned char data );
unsigned char USART0_rx( void );
