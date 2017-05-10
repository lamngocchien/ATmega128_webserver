//----------------------------------------------------------------------------
// Writen by NTTam - PTITHCM
//----------------------------------------------------------------------------
#ifndef UART_H
#define UART_H

#include <avr/pgmspace.h>
//----------------------------------------------------------------------------
#define UART_TX_BUFFER_SIZE		8
#define UART_RX_BUFFER_SIZE		8
//--------------------------------------------------------------------------------------
#ifndef UART_INTERRUPT_HANDLER
#define UART_INTERRUPT_HANDLER	SIGNAL
#endif
//define for ATmega32 register
#define USR UCSR0A
#define UCR UCSR0B
#define UBRR UBRR0L
#define UDR UDR0
#define EICR EICRB
#define USART_RX USART0_RX_vect 
#define USART_TX USART0_TX_vect  
//--------------------------------------------------------------------------------------
void uartInit(unsigned long baudrate);
char uartGetByte();
void uartSendByte(char c);
int printfP(const prog_char *format, ...);
#define printf(format, args...) printfP(PSTR(format), ## args)
//--------------------------------------------------------------------------------------
#endif //UART_H
