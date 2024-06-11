/*
 * timer.c
 *
 * Created: 29-Jan-19 4:07:07 PM
 *  Author: Ranul Deepanayake
 */ 

#include "timer.h"

volatile uint16_t time_units= 0;	//Store the number of milliseconds or microseconds.

/*
ISR increments time value on OC0A compare.
*/
ISR(TIMER0_COMPA_vect){
	time_units++;	//Increment the number of milliseconds or microseconds.
}

/*
Set Timer 0 to count milliseconds. CTC mode with OC0A interrupt is used.
*/
void timer_set_millis(){
	//CTC mode is used.
	TCCR0A= TIMER_OC0A_DISCONNECTED;	//OC0A disconnected.
	TCCR0B= TIMER_MILLIS_PRESCALER;	//Set pre-scaler to 64.
	TIMSK0= TIMER_OC0A_INTERRUPT;	//Set OC0A interrupt.
	OCR0A= TIMER_MILLIS_OC0A;	//249 cycles: 1 millisecond.
	sei();	//Enable global interrupts.
	TCNT0= TIMER_TCNT0_RESET;	//Initialize timer.
}

/*
Set Timer 0 to count microseconds. CTC mode with OC0A interrupt is used.
*/
void timer_set_micros(){
	TCCR0A= TIMER_OC0A_DISCONNECTED;	//OC0A disconnected.
	TCCR0B= TIMER_MICROS_PRESCALER;	//Set pre-scaler to 8.
	TIMSK0= TIMER_OC0A_INTERRUPT;	//Set OC0A interrupt.
	OCR0A= TIMER_MICROS_OC0A;	//1 cycle: 1 microsecond.
	sei();	//Enable global interrupts.
	TCNT0= TIMER_TCNT0_RESET;	//Initialize timer.
}

/*
Get elapsed milliseconds. Can store the maximum value of 65535 milliseconds.
*/
uint16_t timer_get_millis(){
	uint16_t temp;
	cli();		//Temporarily disable global interrupts to prevent inconsistencies in the returned value due to partial writes to 'time_units'.
	temp= time_units;
	sei();		//Re-enable global interrupts.
	return temp;
}

/*
Get elapsed milliseconds. Can store the maximum value of 65535 microseconds.
*/
uint16_t timer_get_micros(){
	uint16_t temp;
	cli();		//Temporarily disable global interrupts to prevent inconsistencies in the returned value due to partial writes of 'time_units'.
	temp= time_units* TIMER_MILLIS_TO_MICROS_MULTIPLIER;
	sei();		//Re-enable global interrupts.
	return temp;
}
