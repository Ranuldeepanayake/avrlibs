/*
 * Millis 328P.c
 *
 * Created: 29-Oct-18 8:53:17 PM
 * Author : Ranul Deepanayake
 * A static library which implements the millis function using Timer0 for the Microchip ATmega328P.
 * This does not break Timer0 functions such as '_delay_ms()'.
 */ 
#include "millis.h"

volatile uint16_t milliSeconds= 0;	//Store the number of milliseconds.

ISR(TIMER0_COMPA_vect){
	milliSeconds++;
}

void setTimer(){
	//CTC mode is used.
	TCCR0A= 0x02;	//OC0A disconnected.
	TCCR0B= 0x03;	//Set pre-scaler to 64.
	TIMSK0= 0x02;	//Set OC0A interrupt.
	OCR0A= 0xF9;	//249 cycles: 1 second.
	sei();	//Enable global interrupts.
	TCNT0= 0;	//Initialize timer.
}

uint16_t millis(){
	//Returns time elapsed in milliseconds. Can store the maximum value of 65535 milliseconds.
	uint16_t temp;
	cli();		//Temporarily disable global interrupts to prevent inconsistencies in the returned value due to partial writes of 'milliseconds'.
	temp= milliSeconds;
	sei();		//Re-enable global interrupts.
	return temp;
}
