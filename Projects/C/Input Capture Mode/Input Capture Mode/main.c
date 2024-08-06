/*
 * Input Capture Mode.c
 *
 * Created: 12-Nov-18 9:19:17 AM
 * Author : Ranul Deepanayake
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart328P.h"

void setCapture();
void setOscillator();


ISR(TIMER0_COMPA_vect){
	PORTD^= 0x40;	//Generate wave for testing the ICP mode.
}

int main(void)
{
    uint16_t capture_1= 0, capture_2= 0;
	uint16_t ticks= 0, result= 0, temp= 0;
	uint16_t max_readable_frequency= 15625;
	double time= 0, time_per_tick= 0.000064, frequency= 0;	//Time per tick according to the pre-scaler set. //Double is the same as Float in avr-gcc.
	char string[10];
	
	setUsart(9600);
	setOscillator();
	setCapture();
	
    while (1) 
    {
		//Read the first sample.
		while(!(TIFR1 & 0x20));	//This bit gets automatically set to one when triggered.
		capture_1= ICR1L;	//Read the low byte first.
		capture_1|= (ICR1H<< 8);	//Read the high byte.
		
		TIFR1= (1<< ICF1);	//Manually clear this flag by *writing a one*. 
		
		//Read the second sample.
		while(!(TIFR1 & 0x20));	//This bit gets automatically set to one when triggered.
		capture_2= ICR1L;	//Read the low byte first.
		capture_2|= (ICR1H<< 8);	//Read the high byte.
		
		ticks= capture_2- capture_1;	//Measure the number of ticks between two external waves (samples) (rise to rise or fall to fall).
		time= ticks* time_per_tick;		//Time between two waves (in seconds).
		frequency= 1/time;				//Frequency of the wave (in hertz).
		result= (uint16_t) frequency;
		
		if(result<= 65535){	//Add provisions for frequencies less than 1Hz.
			//sprintf can't accept.
			if(result<= max_readable_frequency && result> 0){
				//temp= (uint16_t)result;
				//sprintf(string, "%d", result);
				dtostrf(frequency, 8, 3, string);
				print(string); print(" "); println("Hz");
			}else{
				println("Out of measurement range!");
			}
		}else{
			println("Out of measurement range!");
		}
    }
}

void setCapture(){
	//Set up the input capture mode.
	DDRB|= 0x00;	//PB0 is ICP1 (input capture pin).
	TCCR1A= 0x00; //Set control register. Disconnect output and de-select waveform generation.
	TCCR1B= 0xC5;	//Set noise canceler (for better results), trigger on rising edge and clock source set to pre-scaler of 1024.
	TIFR1= 0x20;	//Supposed to be cleared. Why is it set?
}

void setOscillator(){
	//Generate a squarewave to test the input capture mode.
	DDRD|= 0x42;
	TCCR0A= 0x02;
	TCCR0B= 0x05;	//Pre-scaler 1024.
	TIMSK0= 0x02; 
	OCR0A= 255;		//30.44Hz
	sei();
	TCNT0= 0;
}

