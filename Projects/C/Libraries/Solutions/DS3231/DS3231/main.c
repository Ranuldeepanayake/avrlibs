/*
 * DS3231.c
 *
 * Created: 08-Nov-18 11:46:24 PM
 * Author : ranul
 */ 

#include "i2c.h"
#include "ds3231.h"
#include "ssd1306.h"
#include "stringfunctions.h"

void setClock();
void readClock();

char array[10];
char *dow;

int main(void)
{
	char date_time[25];
    i2cSet(I2C_PRESCALER, I2C_BAUD_RATE);
	ssd1306Initialize();
	//setClock();
	ssd1306ClearDisplay();
	ssd1306TransferBuffer();
	
    while (1) 
    {
		/*//ds3231_get_date_time(date_time);
		sprintf2(ds3231_get_hour(), date_time);
		ssd1306Print(date_time, 0, 0);
		sprintf2(ds3231_get_minute(), date_time);
		ssd1306Print(date_time, 20, 0);
		sprintf2(ds3231_get_second(), date_time);
		ssd1306Print(date_time, 40, 0);
		ssd1306TransferBuffer();
		//ssd1306ClearDisplay();
		ssd1306Print("   ", 0, 0);
		ssd1306Print("   ", 20, 0);
		ssd1306Print("   ", 40, 0);*/
		ds3231_get_time_all(date_time, 25);
		ssd1306Print(date_time, 0, 0);
		ds3231_get_date_all(date_time, 25);
		ssd1306Print(date_time, 64, 0);
		ssd1306TransferBuffer();
		ssd1306Print("                                                     ", 0, 0);
		ssd1306Print("                                                     ", 64, 0);
    }
}

void readClock(){
	uint8_t second= 0, minute= 0, hour= 0, day= 0, date= 0, month= 0, year= 0;
	i2cDelayedStart(0x68, I2C_WRITE);
	i2cWrite(0x00);
	i2cDelayedStart(0x68, I2C_READ);
	second= i2cReadByte();
	minute= i2cReadByte();
	hour= i2cReadByte();
	day= i2cReadByte();
	date= i2cReadByte();
	month= i2cReadByte();
	year= i2cReadLastByte();
	i2cStop();
	
	second= ((second>> 4)* 10) + (second & 0x0F);	//Get seconds in tens and ones separately and add them.
	minute= ((minute>> 4)* 10) + (minute & 0x0F);
	hour= (((hour & 0x10)>> 4)* 10)+ (hour & 0x0F);
	//day= day;
	date= ((date>> 4)* 10) + (date & 0x0F);
	month= (((month & 0x10)>> 4)* 10)+ (month & 0x0F);
	year= ((year>> 4)* 10) + (year & 0x0F);
	
	switch (day){
		case 1:
		dow= "Sunday";
		break;
		
		case 2:
		dow= "Monday";
		break;
		
		case 3:
		dow= "Tuesday";
		break;
		
		case 4:
		dow= "Wednesday";
		break;
		
		case 5:
		dow= "Thursday";
		break;
		
		case 6:
		dow= "Friday";
		break;
		
		case 7:
		dow= "Saturday";
		break;
	}
	
}

void setClock(){
	i2cDelayedStart(0x68, I2C_WRITE);
	i2cWrite(0x01);
	/*i2cWrite();*/
	i2cWrite(0x48);
	i2cWrite(0x01);
	i2cWrite(0x06);
	i2cWrite(0x09);
	i2cWrite(0x11);
	i2cWrite(0x18);
	stop();
}

