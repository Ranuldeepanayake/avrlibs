/*
 * Input Capture Interrupt.c
 *
 * Created: 23-Nov-18 6:05:07 PM
 * Author : Ranul Deepanayake
 * Prescaler- 8, Max: 2MHz, Min: 30.51Hz
 * Prescaler- 64, Max: 250KHz, Min: 3.81Hz
 * Prescaler- 256, Max: 62.5KHz, Min: 0.9Hz
 */ 

#include "inputcapture.h"

volatile uint16_t input_capture_1= 0, input_capture_2= 0, input_capture= 0;
volatile uint8_t same_capture= 0, capture_count= 1;

ISR(TIMER1_CAPT_vect){
	if(capture_count== 1){
		input_capture_1= ICR1L;
		input_capture_1|= (ICR1H<< 8);
		same_capture= 1;
	}
	if(capture_count== 2){
		input_capture_2= ICR1L;
		input_capture_2|= (ICR1H<< 8);
		input_capture= input_capture_2- input_capture_1;
		capture_count= 1;
		same_capture= 0;
	}
	if(same_capture== 1){
		capture_count= 2;
	}
	else{
		capture_count= 1;
	}
}

double time= 0, time_per_tick= 0.0000005, frequency= 0;	//Time per tick according to the pre-scaler set. //Double is the same as Float in avr-gcc.

int main(void)
{
	setUsart(9600);
	setCapture();
	setTestWave();
	sei();
	
    while (1) 
    {
		OCR0A= 130;
		sendCapture();
    }
}

void setCapture(){
	TCCR1A= 0x00;
	TCCR1B= 0xC2;	//Pre-scaler 8.
	TIMSK1= 0x20;
	TCNT1= 0;
}

void setTestWave(){
	DDRD|= 0xF2; //Set the LED.
	TCCR0A= 0x81; //Clear on Compare and phase correct PWM.
	TCCR0B= 0x01; //Set waveform generation mode and pre-scaler.
}

void sendCapture(){
	char text[12];
	
	time= input_capture* time_per_tick;
	frequency= 1/time;
	dtostrf(frequency, 10, 2, text);
	println(text);
}

