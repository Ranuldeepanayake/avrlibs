/*
 * source.c
 *
 * Created: 02-Oct-18 4:55:03 PM
 *  Author: ranul
 */ 

#include "header.h"

void testLed(){
	_delay_ms(500);
	PORTB = ~PORTB; 
}

void setDigitalInputs(){
	//Sets bits in register PORTD as inputs and outputs.
	DDRD|= 0b00000010;
	//Sets the bits in register PORTB as outputs.
	DDRB|= 0b11111111;	
	//Set bits in register PIND to activate internal pull-up resistors.
	PORTD|= 0b11111100;
}

void setDigitalInputsSamePort(){
	DDRD |= 0x88;
	PORTD |= 0x08; 
}

uint8_t readPins(){
	return PIND;
}

uint16_t readPins16Bit(){
	return PIND<< 8;
}

void buttonStatus(uint8_t input){
	if(!(input & 0b00000100)){
		PORTB= 0xFF;	
	}
	else{
		PORTB= 0x00;	
	}
}

void buttonStatusInverted(char input){
	bool toggle= 0;
	//A ! can be used to invert
	if(input & (0x04)){
		toggle= 0;
	}
	else{
		toggle= 1;	
	}
	if(toggle== 1){
		PORTB |= 0xFF;
	}
	else{
		PORTB &=~ 0xFF;
	}
}

void buttonStatusSamePort(uint8_t input){
	//Input pin: PD3
	//Output pin: PD7
	bool toggle= 0;
	/*if(input & (0x08)){
		toggle= 0;
	}
	else{
		toggle= 1;
	}
	if(toggle== 1){
		PORTD |= 0x80; 
	}
	else{
		PORTD &= ~0x80;
	}*/
	(input & (0x08))? (toggle= 0): (toggle= 1); 
	toggle== 1? (PORTD |= 0x80): (PORTD &= ~0x80);
}

void buttonStatusSamePortAdvanced(uint16_t input){
	//Input pin: PD3
	//Output pin: PD7
	input >>= 8;
	
	bool toggle= 0;
	(input & (0x08))? (toggle= 0): (toggle= 1);
	toggle== 1? (PORTD |= 0x80): (PORTD &= ~0x80);
} 

