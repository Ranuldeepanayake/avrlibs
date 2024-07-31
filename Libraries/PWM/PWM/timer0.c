/*
 * PWM.c
 *
 * Created: 04-Oct-18 5:26:56 PM
 * Author : Ranul Deeepanayake
 * Demonstrates the use of PWM using timers and counters.
 * Timer modes of operation: Fast PWM- good for communication, phase correct PWM- good for motor and light control, frequency and phase correct PWM.
 * Compare output modes: Disabled, normal, toggle, clear on match (non- inverting) and set on match (inverting). 
 * Timer sets the frequency.
 * Counter sets PWM.
 * ***Fast PWM has twice the frequency of phase correct PWM.	
 
 Normal mode: Fixed PWM, not suitable for waveform generation.
 CTC: Variable PWM width through OCR0A.
 Fast PWM: Variable PWM width through OCR1A. PWM resolution defined by ICR1 or OCR1A.
 Phase correct PWM: Dual slope. Variable PWM width through ICR or OCR1A. Not suitable if TOP changes.
 Phase and frequency correct PWM: Dual slope. Variable PWM width through ICR or OCR1A.
 
 Toggle on match: 50% PWM
 Clear on match and set on match: Variable non-inverted and inverted PWM based on the OCR0x.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pwm.h"

volatile uint8_t totalOverflow= 0;

ISR(TIMER0_OVF_vect){
	totalOverflow++;
	//PORTB ^= 0x08;
}


/*ISR(TIMER0_OVF_vect){
	totalOverflow= 0; //Reset overflow.
	TCNT0 &= 0x00; //Reset timer counter.
	TIFR0&= 0x0;
}*/

int main(void)
{
    //setPWM();
	//setLedInterval();
	//setIntervalPwm();
	setInterruptPwm();
	//setInterruptManual();
	
    while (1) 
    {
		//writePWM();
		//writeLedInterval();
		//writeIntervalPwm();
		writeInterruptPwm();
		//writeInterruptManual();
    }
}

void pwmTimer0set(){
	//Uses timer 0 (8 bit).
	//Uses the timer to vary the voltage of an LED.
	DDRD|= 0x42; //Set the LED.
	TCCR0A|= ((1<< COM0A1) | (1<< WGM01) | (1<< WGM00)); //Set compare output mode for the port (pin) (non- inverting is set) and waveform generation (fast PWM is set).
	TCCR0B|= (1<< CS00); //Set waveform generation mode and pre-scaler.
}

void writePWM(){
	//OCR0A= 220;
	for(int i= 0; i<= 255; i++){
		OCR0A= i;
		_delay_ms(10);
	}
	for(int i= 255; i>= 0; i--){
		OCR0A= i;
		_delay_ms(10);
	}
}

void setLedInterval(){
	//Uses timer 0 (8 bit).
	//Uses the timer to flash an LED with a certain frequency.
	DDRD|= 0x42; //Set LED.
	TCCR0A|= (1<< WGM00); //Set waveform generation (phase correct PWM is set).
	TCCR0B|= ((1<< CS02) | (1<< CS00)); //Set pre-scaler.
	TCNT0&= 0x00; //Initialize counter. ??
}

void writeLedInterval(){
	if(TCNT0>= 250){ //Check the value of the counter.
		PORTD^= 0x40; //Toggle the LED.
		TCNT0&= 0x00; //Reset counter. 
	}
}

void setIntervalPwm(){
	//Uses timer 0 (8 bit).
	//Uses the timer to flash an LED with a certain frequency with a certain duty cycle.
	DDRD|= 0x42; //Set the LED.
	TCCR0A|= ((1<< COM0A1) | (1<< WGM00)); //Set compare output mode for the port (pin) (non- inverting is set) and waveform generation (phase correct PWM is set).
	TCCR0B|= ((1<< CS02)); //Set pre-scaler.
}

void writeIntervalPwm(){
	//Set duty cycle.
	OCR0A= 10;
}

void setInterruptPwm(){
	DDRB|= 0x2F; //Set the LEDs.
	TCCR0A|= 0x83; //Set compare output mode and waveform generation.
	TCCR0B|= 0x05; //Set the pre-scaler.
	TIMSK0|= (1<< 0); //Enable Timer0 interrupts.
	sei(); //Enable global interrupts in SREG.
}

void writeInterruptPwm(){
	//This is a test function which is not necessary for the example.
	//Blinks the LED at a 1.010 second interval.
	if(totalOverflow>= 61){ //1000/ 16.384 ms (max time with 8 bit counter at 15kHz (pres-scaler- 1024)).
		if(TCNT0>= 157){ //10/ 0.064 ms.
			//PORTB ^= 0x20;
			PORTB ^= 0x08;
			totalOverflow= 0; //Reset overflow.
			TCNT0 &= 0x00; //Reset timer counter.
		}
	}
}

//Below two functions haven't been completed.
void setInterruptManual(){
	DDRB|= 0x2F;
	TCCR0A|= 0x83;
	TCCR0B|= 0x04;
	TIMSK0|= (1<< 0); //Enable Timer0 interrupts.
	sei();
}

void writeInterruptManual(){
	totalOverflow++;
	if(totalOverflow>= 244){
		TIFR0|= 0x1;
	}
}

/*
CTC mode.
*/

/*
 * PWM CTC.c
 *
 * Created: 15-Oct-18 11:10:47 AM
 * Author : Ranul Deepanayake
 * Demonstrates CTC mode of timers.
 */ 


ISR(TIMER0_COMPA_vect){
	//Method 1.
	PORTD^= 0x80;
}

int main(void)
{  
	//setTimer();
	setTimerCtcToggle();
	
    while (1) 
    {
		
    }
}

void setTimer(){
	//Method 1. Uses an ISR.
	DDRD|= 0x82; //Set LED.
	TCCR0A= (1<< WGM01); //Select CTC waveform generation mode.
	TCCR0B= ((1<< CS01) | (1<< CS00)); //Set pre-scaler.
	TIMSK0= (1<< OCIE0A);
	OCR0A= 4; //Set to 25 kHz.
	sei(); //Set global interrupts.
	TCNT0= 0; //Initialize counter.
}

void setTimerCtcToggle(){
	//Method 2. Uses toggle on compare match.
	//Does not require an ISR to toggle the compare output pin. 
	DDRD|= 0xF2; //Set LEDs.
	TCCR0A= ((1<< COM0A0) | (1<< WGM01)); //Select toggle on compare match (pin), select CTC waveform generation mode.
	TCCR0B= ((1<< CS01) | (1<< CS00)); //Set pre-scaler.
	TIMSK0= (1<< OCIE0A);
	OCR0A= 4; //Set to 25 kHz.
	sei(); //Set global interrupts.
	TCNT0= 0; //Initialize counter.
}

void timer0Set(uint8_t mode, uint8_t channel, uint8_t output, uint8_t prescaler){
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
		case TIMER_N_MODE_PWM_PHASE_CORRECT_OCRA:
			TCCR0A |= TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCRA;
			TCCR0B |= TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCRA;
			break;
		case TIMER_N_MODE_PWM_FAST_OCRA:
			TCCR0A |= TIMER_N_WGM_01_PWM_FAST_OCRA;
			TCCR0B |= TIMER_N_WGM_2_PWM_FAST_OCRA;
			break;
	}
	
	TCNT0 = 0x00; //Zero out the timer counter.
	
	//Enable interrupts.
	switch(channel){
		case TIMER_N_CHANNEL_A:
			TIMSK0 |= TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE;
			break;
		case TIMER_N_CHANNEL_A:
			TIMSK0 |= TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_ENABLE;
			break;
	}
	
	sei(); //Enable global interrupts.
	
	//Setting the prescaler turns on the timer. Hence, it is to be set as the final step.
	TCCR0B |= prescaler;
}

