/*
 * SPI Test.c
 *
 * Created: 02-Jul-24 12:04:21 PM
 * Author : ranul
 */ 

#include <avr/io.h>
#include "spi.h"
#include <util/delay.h>


int main(void)
{
	spiTest();
	//DDRB = 0xFF;
    while (1) 
    {	
		//PORTB |= (1 << PORTB2);
		//_delay_ms(10);
		//PORTB &= ~(1 << PORTB2);
		//_delay_ms(10);
		//uint16_t a = (uint8_t) DDRB  - __SFR_OFFSET;
		//a += 1;
		//a = PORTB;
    }
}

