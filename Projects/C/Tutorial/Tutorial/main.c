/*
 * Tutorial.c
 *
 * Created: 29-Sep-18 6:47:24 PM
 * Author : Ranul Deepanayake
 * Contains several demonstrations on manipulating pins as outputs on the ATmega328p. 
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "header.h"

#define BLINK_DELAY 150
//#define F_CPU 16000000

int main(void)
{
    /* Replace with your application code */
	//DDRB |= 0b00100000;
	blinkAllSet();
	 
    while (1) 
    {
		blink13Advanced();
		//blinkAll();
		//fadeSingle();
		//pattern();
		//patternBoth();
    }
}

void blink13(){
	PORTB= 0b00100000;
	_delay_ms(BLINK_DELAY);
	PORTB= 0b00000000; 
	_delay_ms(BLINK_DELAY);
}

void blink13Advanced(){
	
	//In binary.
	//uint8_t high= 0b00100000;
	//uint8_t low= 0b00000000;
	//Ha ha. You realized char & byte are the same as uint8_t;
	
	//In hexadecimal.
	uint8_t high= 0x20;
	uint8_t low= 0x00;
	
	
	PORTB= high; 
	_delay_ms(BLINK_DELAY);
	PORTB= low;
	_delay_ms(BLINK_DELAY);
}

void blinkAllSet(){
	DDRB |= 0b11111111;
	DDRC |= 0b00111111;
	DDRD |= 0b00111110;
}

void blinkAll(){
	PORTB |= 0b00111111;
	PORTC |= 0b00111111;
	PORTD |= 0xb3F;
	_delay_ms(BLINK_DELAY);
	PORTB &=~ 0b00111111;
	PORTC &=~ 0b00111111;
	PORTD &=~ 0xbF;
	_delay_ms(BLINK_DELAY);
} 

void fadeSingle(){
	//Complete properly after learning PWM.
	PORTB |= 0x20;
	_delay_ms(BLINK_DELAY);
	PORTB &=~ 0x20;
	_delay_ms(BLINK_DELAY);
}

void pattern(){
	//PORTB= 0b00000001;
	for(int i= 0; i< 8; i++){
		if(i== 0){
			PORTB= 0b00000001;
		}
		else{
			PORTB= PORTB << i;	
		}
		_delay_ms(BLINK_DELAY);
		if(i== 7){
			PORTB= 0b00000000;
		}
		else{
			PORTB= PORTB >> i;
		}
		 _delay_ms(BLINK_DELAY);
	}
}

void patternBoth(){
	PORTB= 0b00000001;
	for(int i= 0; i< 8; i++){
		PORTB= PORTB << i;
		_delay_ms(BLINK_DELAY);
		PORTB= PORTB >> i;
	}
	
	PORTB= 0b01000000;
	for(int i= 0; i< 8; i++){
		PORTB= PORTB >> i;
		_delay_ms(BLINK_DELAY);
		PORTB= PORTB << i;
	}
}

