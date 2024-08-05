#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*
Sets the timer with the specified mode, output pin behavior and prescaler.
*/
void timer1Set(uint8_t mode, uint8_t output, uint8_t prescaler){
	TCCR1A |= output; //Output pin behavior.
	
	//WGM bit setup.
	switch(mode){
		case TIMER_16_MODE_NORMAL:
			TCCR1A |= TIMER_16_WGM_01_NORMAL;
			TCCR1B |= TIMER_16_WGM_23_NORMAL;
			break;
		case TIMER_16_MODE_PWM_PHASE_CORRECT_8_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_CORRECT_8_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_CORRECT_8_BIT;
			break;
		case TIMER_16_MODE_PWM_PHASE_CORRECT_9_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_CORRECT_9_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_CORRECT_9_BIT;
			break;
		case TIMER_16_MODE_PWM_PHASE_CORRECT_10_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_CORRECT_10_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_CORRECT_10_BIT;
			break;
		case TIMER_16_MODE_CTC_OCR:
			TCCR1A |= TIMER_16_WGM_01_CTC_OCR;
			TCCR1B |= TIMER_16_WGM_23_CTC_OCR;
			break;
		case TIMER_16_MODE_PWM_FAST_8_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_FAST_8_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_FAST_8_BIT;
			break;
		case TIMER_16_MODE_PWM_FAST_9_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_FAST_9_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_FAST_9_BIT;
			break;
		case TIMER_16_MODE_PWM_FAST_10_BIT:
			TCCR1A |= TIMER_16_WGM_01_PWM_FAST_10_BIT;
			TCCR1B |= TIMER_16_WGM_23_PWM_FAST_10_BIT;
			break;
		case TIMER_16_MODE_PWM_PHASE_FREQUENCY_CORRECT_ICR:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_FREQUENCY_CORRECT_ICR;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_FREQUENCY_CORRECT_ICR;
			break;
		case TIMER_16_MODE_PWM_PHASE_FREQUENCY_CORRECT_OCR:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_FREQUENCY_CORRECT_OCR;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_FREQUENCY_CORRECT_OCR;
			break;
		case TIMER_16_MODE_PWM_PHASE_CORRECT_ICR:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_CORRECT_ICR;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_CORRECT_ICR;
			break;
		case TIMER_16_MODE_PWM_PHASE_CORRECT_OCR:
			TCCR1A |= TIMER_16_WGM_01_PWM_PHASE_CORRECT_OCR;
			TCCR1B |= TIMER_16_WGM_23_PWM_PHASE_CORRECT_OCR;
			break;
		case TIMER_16_MODE_CTC_ICR:
			TCCR1A |= TIMER_16_WGM_01_CTC_ICR;
			TCCR1B |= TIMER_16_WGM_23_CTC_ICR;
			break;
		case TIMER_16_MODE_PWM_FAST_ICR:
			TCCR1A |= TIMER_16_WGM_01_PWM_FAST_ICR;
			TCCR1B |= TIMER_16_WGM_23_PWM_FAST_ICR;
			break;
		case TIMER_16_MODE_PWM_FAST_OCR:
			TCCR1A |= TIMER_16_WGM_01_PWM_FAST_OCR;
			TCCR1B |= TIMER_16_WGM_23_PWM_FAST_OCR;
			break;
	}
	
	TCNT1 = 0x00; //Zero out the timer counter.
	
	//Setting the prescaler turns on the timer. Hence, it is to be set as the final step.
	TCCR1B |= prescaler;
}

/*
Write to the OCR pin.
*/
void timer1WriteOcr(uint8_t channel, uint8_t value){
	cli();
	switch(channel){
		case TIMER_N_CHANNEL_A:
			OCR1A = value;
			break;
		case TIMER_N_CHANNEL_B:
			OCR1B = value;
			break;
	}
	sei();
}

/*
Write to ICR.
*/
void timer1WriteIcr(uint8_t value){
	cli();
	ICR1 = value;
	sei();
}



void setTimerPwmPhaseCorrect(){
	//Method 1. Configure Timer 1 for phase correct PWM @ 25 kHz.
	DDRB|= 0x06; //Set PWM output pins. 
	TCCR1A= ((1<< COM1A1) | (1<< COM1B1) | (1<< WGM11));  //Set non-inverted PWM on channel A and B and mode 10: phase correct PWM waveform (TOP = ICR1).
	TCCR1B= ((1<< WGM13) | (1<< CS10));  //Set waveform generation mode and pre-scaler.
	ICR1= 319;         // TOP = 319 (25.03 kHz).
	TCNT1= 0; //Initialize timer.
}

void writePwmPhaseCorrect(){
	//Set duty cycle.
	OCR1A= 318;
	OCR1B= 159;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setPwmFast(){
	//Method 2. Configure Timer 1 for fast PWM @ 10.63 kHz.
	DDRB|= 0x06;
	TCCR1A= 0x82;
	TCCR1B= 0x19;
	ICR1= 1500;
	TCNT1= 0;
}

void writePwmFast(){
	//Set duty cycle.
	OCR1A= 150; //The maximum value of OCRnx is the value of ICR. 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setPwmCtc(){
	//Method 3.
	DDRB|= 0x06;
	TCCR1A= 0x40;
	TCCR1B= 0x19;
	ICR1H= 0xF0;
	ICR1L= 0x00; 
	TCNT1= 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setPwmInterrupt(){
	//Method 4.
	DDRB|= 0x06;
	TCCR1A= 0x03;
	TCCR1B= 0x11;
	TIMSK1= (1<< 1); //Enable output compare interrupt.
	OCR1AH= 0x00;
	OCR1AL= 0xFF;
	sei();
	TCNT1= 0; 
}



