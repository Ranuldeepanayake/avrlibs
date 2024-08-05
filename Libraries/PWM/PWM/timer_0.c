#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*
Sets the timer with the specified mode, output pin behavior and prescaler.
*/
void timer0Set(uint8_t mode, uint8_t output, uint8_t prescaler){
	TCCR0A |= output; //Output pin behavior.
	
	//WGM bit setup.
	switch(mode){
		case TIMER_N_MODE_NORMAL:
			TCCR0A |= TIMER_N_WGM_01_NORMAL;
			TCCR0B |= TIMER_N_WGM_2_NORMAL;
			break;
		case TIMER_N_MODE_PWM_PHASE_CORRECT:
			TCCR0A |= TIMER_N_WGM_01_PWM_PHASE_CORRECT;
			TCCR0B |= TIMER_N_WGM_2_PWM_PHASE_CORRECT;
			break;
		case TIMER_N_MODE_CTC:
			TCCR0A |= TIMER_N_WGM_01_CTC;
			TCCR0B |= TIMER_N_WGM_2_CTC;
			break;
		case TIMER_N_MODE_PWM_FAST:
			TCCR0A |= TIMER_N_WGM_01_PWM_FAST;
			TCCR0B |= TIMER_N_WGM_2_PWM_FAST;
			break;
		case TIMER_N_MODE_PWM_PHASE_CORRECT_OCR:
			TCCR0A |= TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCR;
			TCCR0B |= TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCR;
			break;
		case TIMER_N_MODE_PWM_FAST_OCR:
			TCCR0A |= TIMER_N_WGM_01_PWM_FAST_OCR;
			TCCR0B |= TIMER_N_WGM_2_PWM_FAST_OCR;
			break;
	}
	
	TCNT0 = 0x00; //Zero out the timer counter.
	
	//Setting the prescaler turns on the timer. Hence, it is to be set as the final step.
	TCCR0B |= prescaler;
}

void timer0Write(uint8_t channel, uint8_t value){
	switch(channel){
		case TIMER_N_CHANNEL_A:
			OCR0A = value;
			break;
		case TIMER_N_CHANNEL_B:
			OCR0B = value;
			break;
	}
}


