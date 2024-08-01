#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

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