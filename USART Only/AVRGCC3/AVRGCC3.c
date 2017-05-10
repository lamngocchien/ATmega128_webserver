//----------------------------------------------------------------------------
// Writen by NTTam - PTITHCM
//----------------------------------------------------------------------------
#include <avr/io.h>
#include "uart.h"
#include <util/delay.h>
#include "ntAVRnet.h"
//--------------------------------------------------------------------------------------
int	main()
{
	DDRC = 0xff;	//out
	PORTC = 0xff;
	uartInit(UART_BAUDRATE);
	//printf("\r\nNTTam AVR network testing with enc28j60.\r\n");
	//printf("c");
	printf("Ethernet chip init\r\n");
	while(1)
	{
			printf("Lam Ngoc Chien");
			ON1
			_delay_ms(2000);
			OFF1
			_delay_ms(2000);
	}
	return 0;
}

