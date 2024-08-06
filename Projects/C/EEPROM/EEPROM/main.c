/*
 * EEPROM.c
 *
 * Created: 09-Nov-18 8:37:24 PM
 * Author : ranul
 */ 
#include "eeprom.h"

int main(void)
{
	i2cSet(1, I2C_BAUD_RATE);
	setUsart(9600);
	DDRB|= 0x20;	//Set LED.
	
    char temp[10];
	uint8_t status= 100;
	uint8_t image[1]= {0xAA};
	
	/*for(int i= 1; i<= 512; i++){
		status= eepromWritePage(image, 128, 128, i);
		sprintf(temp, "%d", status);
		println(temp);
	}*/
	
	eepromReadPage(image, 128, 1);
	
	
    while (1) 
    {
    }
}

