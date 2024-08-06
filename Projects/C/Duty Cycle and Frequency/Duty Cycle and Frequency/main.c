/*
 * Duty Cycle and Frequency.c
 *
 * Created: 15-Oct-18 3:37:18 PM
 * Author : Ranul Deepanayake
 * Demonstrates the use of Timer 1 to manipulate signal frequency and duty cycle.
 */ 

#include <avr/io.h>

void setTimer();
void writePwmPhaseCorrect();

int main(void)
{
	setTimer();

    while (1) 
    {
		writePwmPhaseCorrect();
    }
}

void setTimer(){
	// Configure Timer 1 for PWM @ 25 kHz.
	DDRB|= 0x06; //Set PWM output pins.
	TCCR1A= ((1<< COM1A1) | (1<< COM1B1) | (1<< WGM11));  //Set non-inverted PWM on channel A and B and mode 10: phase correct PWM waveform (TOP = ICR1).
	TCCR1B= ((1<< WGM13) | (1<< CS10));  //Set waveform generation mode and pre-scaler.
	ICR1= 319;         // TOP = 319 (25.03 kHz).
	TCNT1= 0; //Initialize timer.
}

void writePwmPhaseCorrect(){
	OCR1AL= 50;
	OCR1AH= 150;
	OCR1B= 75;
}


