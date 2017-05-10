//----------------------------------------------------------------------------
// Writen by NTTam - PTITHCM
//----------------------------------------------------------------------------
#ifndef NTAVRNET_H
#define NTAVRNET_H

#ifndef F_CPU
	#define F_CPU        8000000	// Cho toc do la 12MHz
#endif //F_CPU
#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	//So chu ky lenh trong 1 micro giay

#define LO8(x) ((x)&0xFF)
#define HI8(x) (((x)>>8)&0xFF)

#define UART_BAUDRATE	9600l

#define TIMER_PRESCALE		1024
#define TIMER1_INTERVAL		(F_CPU/TIMER_PRESCALE)

#define IPDOT(a,b,c,d)	((unsigned long)((unsigned char)a)<<24)+((unsigned long)((unsigned char)b)<<16)+((unsigned long)((unsigned char)c)<<8)+(unsigned char)d//((a<<24)|(b<<16)|(c<<8)|(d))

//Define IP
#define IPADDRESS			IPDOT(0,0,0,0)
#define NETMASK				IPDOT(0,0,0,0)
#define GATEWAY				IPDOT(0,0,0,0)

//Define  MAC
#define ETHADDR0	'0'
#define ETHADDR1	'F'
#define ETHADDR2	'F'
#define ETHADDR3	'I'
#define ETHADDR4	'C'
#define ETHADDR5	'E'

//Define Device
////#define setbit(port,pin) port |= 1<<pin 
////#define clrbit(port,pin) port &= ~(1<<pin)
////#define ON1    clrbit(PORTC,PORTC7);
////#define ON2    clrbit(PORTC,PORTC6);
////#define ON3    clrbit(PORTC,PORTC5);
////#define ON4    clrbit(PORTC,PORTC4);
////#define ON_ALARM	clrbit(PORTC,PORTC3);
////#define OFF1    setbit(PORTC,PORTC7);
////#define OFF2    setbit(PORTC,PORTC6);
////#define OFF3    setbit(PORTC,PORTC5);
////#define OFF4    setbit(PORTC,PORTC4);
////#define OFF_ALARM	setbit(PORTC,PORTC3);

#define setbit(port,pin) port |= 1<<pin 
#define clrbit(port,pin) port &= ~(1<<pin)
#define OFF1    clrbit(PORTC,PORTC0);
#define OFF2    clrbit(PORTC,PORTC1);
#define OFF3    clrbit(PORTC,PORTC2);
#define OFF4    clrbit(PORTC,PORTC3);
#define ON1    setbit(PORTC,PORTC0);
#define ON2    setbit(PORTC,PORTC1);
#define ON3    setbit(PORTC,PORTC2);
#define ON4    setbit(PORTC,PORTC3);
//Define Password HTTP Server
#define HTTP_AUTH_STRING "admin:1234"

//Define Ham Cons
void showip (void);
uint8_t GetState(uint8_t val);
#endif //NTAVRNET_H
