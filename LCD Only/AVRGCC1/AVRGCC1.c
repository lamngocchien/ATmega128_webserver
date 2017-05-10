/*
 * AVRGCC1.c
 *
 * Created: 4/13/2014 12:52:13 PM
 *  Author: Sony Vaio E
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
//Define Device
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
int main(void)
{
	DDRC = 0xff;	//out
	PORTC = 0xf;
	init_LCD(); 
	clr_LCD(); 
	//Test 16xx
	move_LCD(1,1);
	print_LCD("Demo of the LCD");
	move_LCD(2,1);
	print_LCD("2x16 LCD Display");
	move_LCD(3,1);
	//print_LCD("HELLO EVERY GOOD");
	//move_LCD(4,1);
	//print_LCD("LAM NGOC CHIEN");
	//Test 20xx
	//print_LCD("Demo of the LCD ABCD");
	//move_LCD(2,1);
	//print_LCD("2x16 LCD Display ABC");
	//move_LCD(1,1);
	//print_LCD("TEST LCD ");
	//move_LCD(2,1);
	//print_LCD("ATMEGA128");
	
    while(1)
    {
		//TODO:: Please write your application code 
		ON1
		_delay_ms(1000);
		OFF1
		_delay_ms(1000);
		ON2
		_delay_ms(1000);
		OFF2
		_delay_ms(1000);
		ON3
		_delay_ms(1000);
		OFF3
		_delay_ms(1000);
		ON4
		_delay_ms(1000);
		OFF4
		PORTC = 0xff;
		_delay_ms(1000);
		PORTC = 0x00;
        _delay_ms(1000);
		PORTC = 0xff;
		_delay_ms(1000);
		PORTC = 0x00;
        _delay_ms(1000);
		PORTC = 0xff;
		_delay_ms(1000);
		PORTC = 0x00;
		PORTC = 0xff;
		_delay_ms(1000);
		PORTC = 0x00;
        _delay_ms(1000);
        _delay_ms(1000);
    }
}
