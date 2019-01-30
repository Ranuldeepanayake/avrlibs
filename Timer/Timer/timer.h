/*
 * timer.h
 *
 * Created: 29-Jan-19 4:07:20 PM
 * Author: Ranul Deepanayake
 * Timer library for the ATmega328P clocked at 16MHz. Uses Timer 0.
 * Can measure elapsed milliseconds and microseconds (both modes can't run at the same time).
 */ 


#ifndef TIMER_H_
#define TIMER_H_

//Includes.
#include <avr/io.h>
#include <avr/interrupt.h>

//Attributes.
#define TIMER_OC0A_DISCONNECTED 0x02
#define TIMER_OC0A_INTERRUPT 0x02
#define TIMER_TCNT0_RESET 0x00
#define TIMER_MILLIS_PRESCALER 0x03
#define TIMER_MICROS_PRESCALER 0x03
#define TIMER_MILLIS_OC0A 0xF9
#define TIMER_MICROS_OC0A 0x01
#define TIMER_MILLIS_TO_MICROS_MULTIPLIER 8

//Functions.
//Set timer to count milliseconds.
void timer_set_millis();
//Set timer to count microseconds.
void timer_set_micros();		
//Get milliseconds.
uint16_t timer_get_millis();
//Get microseconds.				 
uint16_t timer_get_micros();

/*
Example implementation.

#include <avr/io.h>
#include "timer.h"

int main(void)
{
	uint16_t previous_time= 0, current_time= 0, on_time= 500, total_time= 1000;
	timer_set_millis();
	DDRB|= 0x20;
	
	while (1)
	{
		current_time= timer_get_millis();
		if((current_time- previous_time)>= total_time){
			PORTB|= 0x20;
			previous_time= timer_get_millis();
		}
		
		if(current_time- previous_time>= on_time){
			PORTB&= ~(0x20);
		}
	}
}

*/

#endif /* TIMER_H_ */