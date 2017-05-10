/*
 * AVRGCC1.c
 *
 * Created: 4/13/2014 12:52:13 PM
 *  Author: Sony Vaio E
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "lcd.h"
#include "1wire.h"
uint8_t ds18b20, digit;
void lcd_show_number(uint8_t val)
{
	if(val /100 == 0 && (val %100) /10 != 0)	putChar_LCD((val %100) /10+48);
	else if(val /100 != 0) {putChar_LCD(val /100+48);	putChar_LCD((val %100) /10+48);}
	putChar_LCD((val %100) %10+48);
}
void showtemp ()
{
	move_LCD(1,10);
	therm_read_temperature();
	lcd_show_number(ds18b20);
	print_LCD(".");
	putChar_LCD(digit/10 +48);
	putChar_LCD(digit%10 +48);
	putChar_LCD(0xdf);	print_LCD("C");	print_LCD(" ");
	//check_auto();
	//alarm();
}
void	SystemInit()
{
	//LCD
	init_LCD();
	clr_LCD();
	
	//Set PORT
	DDRC = 0xff;	//out
	PORTC = 0xff;
	DDRB = 0x00;	//in	
	PORTB = 0xff;
	DDRD = 0x00;
	PORTD = 0xff;
	//Timer
	//timer1Init();
	
	//UART
//	uartInit(UART_BAUDRATE);
	
	////RTC
	//set_clock();
	//TWI_DS1307_wadr(0x00); //set dia chi ve 0
	//_delay_ms(1);			//cho DS1307 xu li 
	//TWI_DS1307_rblock(tData,7); //doc ca khoi thoi gian (7 bytes)	
	//Display(); // hien thi ket qua len LCD	
}
int main(void)
{
	//init_LCD(); 
	//clr_LCD(); 
	//Test 16xx
	//move_LCD(1,1);
	//print_LCD("Demo of the LCD");
	//move_LCD(2,1);
	//print_LCD("2x16 LCD Display");
	//move_LCD(3,1);
	//print_LCD("HELLO EVERY GOOD");
	//move_LCD(4,1);
	//print_LCD("LAM NGOC CHIEN");
	//Test 20xx
	//print_LCD("Demo of the LCD ABCD");
	//move_LCD(2,1);
	//print_LCD("2x16 LCD Display ABC");
	//move_LCD(1,1);
	SystemInit();
	print_LCD("TEST LCD ");
	move_LCD(2,1);
	print_LCD("ATMEGA128");
    while(1)
    {
		showtemp ();
        //TODO:: Please write your application code 
    }
}
