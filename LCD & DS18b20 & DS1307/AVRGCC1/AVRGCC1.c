/*
 * AVRGCC1.c
 *
 * Created: 4/13/2014 12:52:13 PM
 *  Author: Sony Vaio E
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <stdio.h>
#include "lcd.h"
#include "rtc.h"
#include "1wire.h"
uint8_t ds18b20, digit;
volatile uint8_t	Second=30, Minute=16, Hour=20, Day=2, Date=15, Month=4, Year=13, Mode=0, AP=1;
volatile  uint8_t tData[7];
char dis[5];
//Cac ham theo RTC
uint8_t BCD2Dec(uint8_t BCD){
	uint8_t L, H;
	L=BCD & 0x0F;
	H=(BCD>>4)*10;
	return (H+L);
}
uint8_t Dec2BCD(uint8_t Dec){
	uint8_t L, H;
	L=Dec % 10;
	H=(Dec/10)<<4;
	return (H+L);
}
void	Display	()
{ 
	Second 	= BCD2Dec(tData[0] & 0x7F);
	Minute 	= BCD2Dec(tData[1]);
	
	if (Mode !=0) 	Hour = BCD2Dec(tData[2] & 0x1F); //mode 12h
	else 		  	Hour = BCD2Dec(tData[2] & 0x3F); //mode 24h	
	Day		= BCD2Dec(tData[3]);
	Date   	= BCD2Dec(tData[4]);
	Month	= BCD2Dec(tData[5]);
	Year	= BCD2Dec(tData[6]);
	
	clr_LCD();		//xoa LCD
	//Xuat gio:phut:giay
	//Xuat Hour
	sprintf(dis, "%i",Hour);
	move_LCD(1,1);  if (Hour < 10) putChar_LCD(' ');print_LCD(dis); 
	move_LCD(1,3); putChar_LCD(':');
	//Xuat Minute
	sprintf(dis, "%i",Minute); 
	move_LCD(1,4); if (Minute < 10) putChar_LCD('0');		print_LCD(dis); 
	move_LCD(1,6);	putChar_LCD(':');
	//Xuat Second
	sprintf(dis, "%i",Second); 
	move_LCD(1,7); if (Second<10) putChar_LCD('0');		print_LCD(dis); 
	if (Mode != 0)
	{ 
		//mode 12h
		move_LCD(1,1);
		if (bit_is_set(tData[2],5))  putChar_LCD('P'); //kiem tra bit AP, if AP=1
		else putChar_LCD('A');
	}
	//Xuat nam-thang-ngay
	move_LCD(2,1);
	switch (Day)
		{
		case 1:		print_LCD("Sun");	break;
		case 2:		print_LCD("Mon");	break;
		case 3:		print_LCD("Tue");	break;
		case 4:		print_LCD("Wed");	break;
		case 5:		print_LCD("Thu");	break;
		case 6:		print_LCD("Fri");	break;
		case 7:		print_LCD("Sat");	break;
		}
	
	//Xuat Date
	sprintf(dis, "%i",Date);  
	move_LCD(2,5); 
	if (Date<10)	putChar_LCD(' '); 
	print_LCD(dis);
	move_LCD(2,7);	putChar_LCD('/');//dau cach 1
	
	//Xuat Month
	sprintf(dis, "%i",Month);  
	if (Month<10)	putChar_LCD('0'); 
	print_LCD(dis); 
	move_LCD(2,10); putChar_LCD('/');//dau cach 2

	//Xuat Year
	putChar_LCD('2'); putChar_LCD('0');//xuat so '20'
	sprintf(dis, "%i",Year);	
	move_LCD(2,13);
	if (Year < 10) putChar_LCD('0'); // neu nam <10, in them so 0 ben trai, vi du 09 
	print_LCD(dis);	
}
void set_clock()
{
	unsigned char temp, flag = 1;// mac dinh la cho phep ghi gia tri thoi gian vao ds1307
	//Kiem tra first time qua byte 0x00 trong eeprom da duoc ghi gia tri bao gio chua
	while(!eeprom_is_ready());
	temp = eeprom_read_byte(0); // gia tri mac dinh ban dau cua byte 0 trong eeprom la 0xff = 255
	_delay_ms(1);
	if(temp != 240)
		{
			while(!eeprom_is_ready());
			eeprom_write_byte(0,240); // danh dau lan ghi dau tien 0xf0
			_delay_ms(1);
		}
	else
			flag = 0;
	//Ghi tri ghi vao DS1307
	if(flag)
	{
		//must_edit = 0; // Su dung de tinh chinh thoi gian sau nay
		tData[0] = Dec2BCD(Second); 
		tData[1] = Dec2BCD(Minute); 
		if (Mode != 0) 
			tData[2] = Dec2BCD(Hour)|(Mode<<6)|(AP<<5); //mode 12h
		else 
			tData[2] = Dec2BCD(Hour);
		tData[3] = Dec2BCD(Day);
		tData[4] = Dec2BCD(Date);
		tData[5] = Dec2BCD(Month); 
		tData[6] = Dec2BCD(Year); 		
		TWI_Init(); //khoi dong TWI		
		TWI_DS1307_wblock(0x00, tData, 7); //ghi lien tiep cac bien thoi gian vao DS1307
		_delay_ms(1);	//cho DS1307 xu li 
	}
}
void showclock()
{
	TWI_DS1307_wadr(0x00); 				//set dia chi ve 0
	_delay_ms(1);		   				//cho DS1307 xu li 
	TWI_DS1307_rblock(tData,1); 	//doc ca khoi thoi gian (7 bytes)		
	//hien thi ket qua len LCD
	if(BCD2Dec(tData[0]) !=Second)
	{ 	
		//chi hien thi ket qua khi da qua 1s
		Second=BCD2Dec(tData[0] & 0x7F);
		sprintf(dis, "%i",Second); 
		move_LCD(1,7); 
		if (Second < 10)	putChar_LCD('0');	print_LCD(dis);
		if (Second == 0) 
		{
			TWI_DS1307_wadr(0x00); 				//set dia chi ve 0
			_delay_ms(1);		   				//cho DS1307 xu li 
			TWI_DS1307_rblock(tData,7); 	//doc ca khoi thoi gian (7 bytes)	
			Display(); 		//moi phut cap nhat 1 lan		
		}
		//if(Second == 1)	active_countdown();	
		//if(Second %5 == 0) showip();
		showtemp();		
	}	
}

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
	timer1Init();
	
	//UART
//	uartInit(UART_BAUDRATE);
	
	//RTC
	set_clock();
	TWI_DS1307_wadr(0x00); //set dia chi ve 0
	_delay_ms(1);			//cho DS1307 xu li 
	TWI_DS1307_rblock(tData,7); //doc ca khoi thoi gian (7 bytes)	
	Display(); // hien thi ket qua len LCD	
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
	//print_LCD("TEST LCD ");
	//move_LCD(2,1);
	//print_LCD("ATMEGA128");
    while(1)
    {
		//showtemp ();
        //TODO:: Please write your application code 
    }
}
