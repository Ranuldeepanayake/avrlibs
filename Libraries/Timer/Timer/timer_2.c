#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

volatile uint16_t _time_units = 0; //Store the number of milliseconds or microseconds.

/*
ISR increments the time unit value on OCR2A match.
*/
ISR(TIMER2_COMPA_vect){
	_time_units++;	//Increment the number of milliseconds or microseconds.
}

/*
Sets the timer with the specified mode, output pin behavior and prescaler.
*/
void timer2Set(uint8_t mode, uint8_t output, uint8_t prescaler){
	TCCR2A |= output; //Output pin behavior.
	
	//WGM bit setup.
	switch(mode){
		case TIMER_N_MODE_NORMAL:
			TCCR2A |= TIMER_N_WGM_01_NORMAL;
			TCCR2B |= TIMER_N_WGM_2_NORMAL;
			break;
		case TIMER_N_MODE_PWM_PHASE_CORRECT:
			TCCR2A |= TIMER_N_WGM_01_PWM_PHASE_CORRECT;
			TCCR2B |= TIMER_N_WGM_2_PWM_PHASE_CORRECT;
			break;
		case TIMER_N_MODE_CTC:
			TCCR2A |= TIMER_N_WGM_01_CTC;
			TCCR2B |= TIMER_N_WGM_2_CTC;
			break;
		case TIMER_N_MODE_PWM_FAST:
			TCCR2A |= TIMER_N_WGM_01_PWM_FAST;
			TCCR2B |= TIMER_N_WGM_2_PWM_FAST;
			break;
		case TIMER_N_MODE_PWM_PHASE_CORRECT_OCR:
			TCCR2A |= TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCR;
			TCCR2B |= TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCR;
			break;
		case TIMER_N_MODE_PWM_FAST_OCR:
			TCCR2A |= TIMER_N_WGM_01_PWM_FAST_OCR;
			TCCR2B |= TIMER_N_WGM_2_PWM_FAST_OCR;
			break;
	}
	
	TCNT2 = 0x00; //Zero out the timer counter.
	
	//Setting the prescaler turns on the timer. Hence, it is to be set as the final step.
	TCCR2B |= prescaler;
}

void timer2Write(uint8_t channel, uint8_t value){
	switch(channel){
		case TIMER_N_CHANNEL_A:
		OCR2A = value;
		break;
		case TIMER_N_CHANNEL_B:
		OCR2B = value;
		break;
	}
}

/*
Set Timer 2 to count microseconds. CTC mode with OCR2A match interrupt is used.
Accurate between 1 ms - ??
*/
void timer2SetMillis(){
	TCCR2A = TIMER_N_MODE_CTC;	//OC2N disconnected, CTC mode.
	TIMSK2 = TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE;	//Set OC0A interrupt.
	OCR2A = TIMER_MILLIS_OCR;	//249 cycles: 1 millisecond.
	TCNT2= 0x00;	//Initialize timer.
	
	sei();	//Enable global interrupts.
	TCCR2B = TIMER_N_PRESCALER_64;	//Set prescaler to 64.
}

/*
Set Timer 2 to count microseconds. CTC mode with OCR2A match interrupt is used.
Accurate between 100 us (0.1 ms) - 65,000 us (65 ms).
*/
void timer2SetMicros(){
	TCCR2A = TIMER_N_MODE_CTC;	//OC2N disconnected, CTC mode.
	TIMSK2 = TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE;	//Set OC0A interrupt.
	OCR2A = TIMER_MICROS_OCR;	//1 cycle: 1 microsecond.
	TCNT2 = 0x00;	//Initialize timer.
	
	sei();	//Enable global interrupts.
	TCCR2B= TIMER_N_PRESCALER_64;	//Set prescaler to 64.
}

/*
Get elapsed milliseconds. Can store the maximum value of 65535 milliseconds.
*/
uint16_t timer2GetMillis(){
	uint16_t temp;
	cli();		//Temporarily disable global interrupts to prevent inconsistencies in the returned value due to partial writes to 'time_units'.
	temp = _time_units;
	sei();		//Re-enable global interrupts.
	return temp;
}

/*
Get elapsed microseconds. Can store the maximum value of 65535 microseconds.
*/
uint16_t timer2GetMicros(){
	uint16_t temp;
	cli();		//Temporarily disable global interrupts to prevent inconsistencies in the returned value due to partial writes of 'time_units'.
	temp = _time_units * TIMER_MICROS_MULTIPLIER;
	sei();		//Re-enable global interrupts.
	return temp;
}