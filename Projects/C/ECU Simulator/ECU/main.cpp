/*
 * ECU.cpp
 *
 * Created: 12-Jun-24 1:28:02 PM
 * Author : ranul
 */ 

#include <avr/io.h>
extern "C" {
	#include "i2c.h"
} 

void setLed(){
	DDRB|= 0x20;
}

void blinkLed(){
	PORTB^= 0x20;
}

int main(void)
{
	setLed();
	i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
	
    while (1) 
    {
		i2cDelayedStart(0x05, I2C_WRITE);
		i2cWrite(10);
		i2cDelayedStart(0x05, I2C_READ);
		i2cReadByte();
		i2cReadLastByte();
		i2cStop();
		blinkLed();
    }
}


