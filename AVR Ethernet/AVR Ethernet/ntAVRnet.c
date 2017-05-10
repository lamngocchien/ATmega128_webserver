//----------------------------------------------------------------------------
// Writen by NTTam - PTITHCM
//----------------------------------------------------------------------------
#include <avr/io.h>
#include "ethernet.h"
#include "ip.h"
#include "arp.h"
#include "timer.h"
#include "uart.h"
#include "dhcp.h"
#include "tcp.h"
#include "http.h"
#include "ntAVRnet.h"
#include "lcd.h"
extern struct ipConfig IpMyConfig;
//Ham DK TB
void off_device (uint8_t val)
{
	switch (val)
		{
			case 1:		OFF1	break;
			case 2: 	OFF2	break;
			case 3: 	OFF3	break;
			case 4: 	OFF4	break;
			default: break;
		}
}
void on_device (uint8_t val)
{
	switch (val)
		{
			case 1:		ON1		break;
			case 2: 	ON2		break;
			case 3: 	ON3		break;
			case 4: 	ON4		break;
			default: break;
		}
}
uint8_t GetState(uint8_t val) // =240 ON =15 OFF
{
	switch (val)
	{
			case 1: 	if(bit_is_clear(PORTC,PINC7))	return 240;	else	return 15;		break;
			case 2: 	if(bit_is_clear(PORTC,PINC6))	return 240;	else	return 15;		break;
			case 3: 	if(bit_is_clear(PORTC,PINC5))	return 240;	else	return 15;		break;
			case 4: 	if(bit_is_clear(PORTC,PINC4))	return 240;	else	return 15;		break;
			default: break;
	}
}

//Pha khai bao
//Xuat 1 so co 3 chu so
void lcd_show_number(uint8_t val)
{
	if(val /100 == 0 && (val %100) /10 != 0)	putChar_LCD((val %100) /10+48);
	else if(val /100 != 0) {putChar_LCD(val /100+48);	putChar_LCD((val %100) /10+48);}
	putChar_LCD((val %100) %10+48);
}
//In ip
void network_out(unsigned long ipaddr, uint8_t kind)
{
	switch(kind)
	{
		case 1: print_LCD("IP:");	break;
		case 2:	print_LCD("SN:");	break;
		case 3:	print_LCD("GW:");	break;
		default: break;
	}
	if(((unsigned char*)&ipaddr)[3] == 0)	{print_LCD("0.0.0.0");}
	else
	{
		lcd_show_number(((unsigned char*)&ipaddr)[3]);	putChar_LCD('.');
		lcd_show_number(((unsigned char*)&ipaddr)[2]);	putChar_LCD('.');
		lcd_show_number(((unsigned char*)&ipaddr)[1]);	putChar_LCD('.');
		lcd_show_number(((unsigned char*)&ipaddr)[0]);
	}	
}
//Xuat ip ra LCD
void showip(void)
{
	move_LCD(1,1);	network_out(IpMyConfig.ip,1);
	move_LCD(2,1);	network_out(IpMyConfig.netmask,2);
}
//----------------------------------------------------------------------------
void netInit(unsigned long ipaddress, unsigned long netmask, unsigned long gatewayip)
{
	// init network device driver
	#ifdef NETSTACK_DEBUG
	printf("Initializing Network Device\r\n");
	#endif
	ethInit();
	// init ARP
	#ifdef NETSTACK_DEBUG
	printf("Initializing ARP cache\r\n");
	#endif
	arpInit();
	// init addressing
	#ifdef NETSTACK_DEBUG
	printf("Initializing Addressing\r\n");
	#endif
	ipSetConfig(ipaddress, netmask, gatewayip);
}
//--------------------------------------------------------------------------------------
void	PrintIPConfig()
{
	printf("MAC Address: ");		ethPrintAddr(&IpMyConfig.ethaddr);		printf("\n\r");
	printf("IP  Address: ");		ipPrintAddr(IpMyConfig.ip);				printf("\n\r");
	printf("Subnet Mask: ");		ipPrintAddr(IpMyConfig.netmask);		printf("\n\r");
	printf("Default Gateway: ");	ipPrintAddr(IpMyConfig.gateway);		printf("\n\r");
}
//--------------------------------------------------------------------------------------
void	SystemInit()
{
	init_LCD();
	clr_LCD();
	DDRC = 0xff;	//out
	PORTC = 0xff;
	DDRB = 0x00;	//in	
	PORTB = 0xff;
	DDRD = 0x00;
	PORTD = 0xff;
	timer1Init();
	uartInit(UART_BAUDRATE);
}
//Cau hinh Ethernet
void EthernetInit()
{	
	printf("\r\nNTTam AVR network testing with enc28j60.\r\n");
	printf("Initializing Network Interface and Stack\r\n");
	printf("Ethernet chip init\r\n");
	IpMyConfig.ethaddr.addr[0] = ETHADDR0;
	IpMyConfig.ethaddr.addr[1] = ETHADDR1;
	IpMyConfig.ethaddr.addr[2] = ETHADDR2;
	IpMyConfig.ethaddr.addr[3] = ETHADDR3;
	IpMyConfig.ethaddr.addr[4] = ETHADDR4;
	IpMyConfig.ethaddr.addr[5] = ETHADDR5;
	IpMyConfig.ip = IPADDRESS;
	IpMyConfig.netmask = NETMASK;
	IpMyConfig.gateway = GATEWAY;
	netInit(IpMyConfig.ip, IpMyConfig.netmask, IpMyConfig.gateway);
	PrintIPConfig();
	printf("Getting IP Address....\r\n");
	if(IpMyConfig.ip == 0x00000000){
		dhcpInit();
	}
	TCPCreateSession(80,httpDataIn);
	//httpInit();
}
void Demo (void)
{
	ON1
	ON3
}
//--------------------------------------------------------------------------------------
int	main()
{
	SystemInit();
	EthernetInit();
	Demo();
	while(1)
	{
		//if(IpMyConfig.ip == 0x00000000)	dhcpInit();
		ethService();
		dhcpService();
	}
	return 0;
}

