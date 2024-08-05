/*
------------------
Modes of operation
------------------
Refer this table when setting up the combination of waveform generation and output pin behavior.

Normal mode:	Fixed PWM fixed frequency. Not suitable for waveform generation (through ISRs).
				Toggle on match: 50% PWM at the prescaled frequency. For frequency generation only. Frequency not adjustable through OCR0A.
				Clear on match and set on match: No effect.
 
CTC:	Variable frequency through OCR0A. Has the highest frequency among all modes.
		Toggle on match: 50% PWM at the prescaled frequency. Frequency adjustable through OCR0A. For frequency generation only. Doesn't work with OCR0B for OC0B, works with OCR0A for OC0B.
		Clear on match and set on match: No effect.
		
Fast PWM:	Variable PWM through OCR0A with single slope. Suitable for LED control and servo control etc. Frequency is higher than phase correct PWM.
			Toggle on match: No effect.
			Clear on match and set on match: Variable non-inverted and inverted PWM based on the OCR0x value at the prescaled frequency. For PWM generation only.
 
Fast PWM with OCRnx:	Variable frequency through OCR0A with single slope. Frequency is higher than phase correct PWM.
						Toggle on match: 50% PWM at the prescaled frequency. Frequency adjustable through OCR0A. For frequency generation only. Doesn't work with OCR0B for OC0B nor with OCR0A for OC0B.
						Clear on match and set on match: No effect.
 
Phase correct PWM: Variable PWM through OCR0A or ICR with dual slope. Not suitable if TOP (OCR0A) changes. Suitable for motor control and etc.
					Toggle on match: No effect.
					Clear on match and set on match: Variable non-inverted and inverted PWM based on the OCR0x value at the prescaled frequency. Frequency not adjustable through OCR0A.
 
Phase correct PWM with OCRnx:	Variable frequency through OCR0A or ICR with dual slope. Not suitable if TOP (OCR0A) changes. Suitable for motor control and audio etc.
								Toggle on match: 50% PWM at the prescaled frequency. Frequency adjustable through OCR0A. For frequency generation only. Doesn't work with OCR0B for OC0B nor with OCR0A for OC0B.
								Clear on match and set on match: No effect.
 
Phase and frequency correct PWM: Dual slope. Variable PWM width through ICR or OCR1A?? (Not tested).


***On modes where the TOP is set with OCRxA, pin OCxB will also be controlled by the OCRxA value. On other modes, pin OCxB will be controlled by the OCRxB value.

 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

/*
Set the prescaler. Useful for changing the prescaler after the initial set up.
*/
void timerSetPrescaler(uint8_t timer, uint8_t prescaler){
	switch(timer){
		case TIMER_0:
			TCCR0B &= ~(0x07);
			TCCR0B |= prescaler;
			break;
		case TIMER_1:
			TCCR1B &= ~(0x07);
			TCCR1B |= prescaler;
			break;
		case TIMER_2:
			TCCR2B &= ~(0x07);
			TCCR2B |= prescaler;
			break;
	}
}

/*
Enable specific timer interrupts such as overflow and compare match.
Enables global interrupts too.
*/
void timerEnableInterrupts(uint8_t timer, uint8_t interrupt){
	switch(timer){
		case TIMER_0:
			TIMSK0 |= interrupt;
			break;
		case TIMER_1:
			TIMSK1 |= interrupt;
			break;
		case TIMER_2:
			TIMSK2 |= interrupt;
			break;
	}
	
	sei(); //Enable global interrupts.
}

/*
Disable specific timer interrupts such as overflow and compare match.
Does not disable global interrupts.
*/
void timerDisableInterrupts(uint8_t timer, uint8_t interrupt){
	switch(timer){
		case TIMER_0:
			TIMSK0 &= ~interrupt;
			break;
		case TIMER_1:
			TIMSK1 &= ~interrupt;
			break;
		case TIMER_2:
			TIMSK2 &= ~interrupt;
			break;
	}
}

/*
Sets the data direction of the OCnx pins as output. 
*/
void timerSetOutputPins(uint8_t timer, uint8_t channel){
	switch(timer){
		case TIMER_0:
			switch(channel){
				case TIMER_N_CHANNEL_A:
				_SFR_IO8(TIMER_0_PIN_A_DDR_REGISTER) |= TIMER_0_PIN_A;
				break;
				case TIMER_N_CHANNEL_B:
				_SFR_IO8(TIMER_0_PIN_B_DDR_REGISTER) |= TIMER_0_PIN_B;
				break;
			}
			break;
		
		case TIMER_1:
			switch(channel){
				case TIMER_N_CHANNEL_A:
				_SFR_IO8(TIMER_1_PIN_A_DDR_REGISTER) |= TIMER_1_PIN_A;
				break;
				case TIMER_N_CHANNEL_B:
				_SFR_IO8(TIMER_1_PIN_B_DDR_REGISTER) |= TIMER_1_PIN_B;
				break;
			}
			break;
		
		case TIMER_2:
			switch(channel){
				case TIMER_N_CHANNEL_A:
				_SFR_IO8(TIMER_2_PIN_A_DDR_REGISTER) |= TIMER_2_PIN_A;
				break;
				case TIMER_N_CHANNEL_B:
				_SFR_IO8(TIMER_2_PIN_B_DDR_REGISTER) |= TIMER_2_PIN_B;
				break;
			}
			break;
	}
}
