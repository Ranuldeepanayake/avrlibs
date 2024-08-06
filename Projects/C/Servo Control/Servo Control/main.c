/*
 * Servo Control.c
 *
 * Created: 13-Oct-18 8:03:11 PM
 * Author : Ranul Deepanayake
 * Demonstrates the use of servos.
 * Most servos require a PWM input signal of a specific frequency.
 * This code is for servos with a 50 Hz input signal frequency.
 * The voltage range which has to be generated through PWM is 87.5 mV (low) to 502 mV (high).
 * Servos have a mechanical operational range of 180 degrees.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY 25

enum bool{false, true};
typedef int bool;

const float inMin= 0.0;
const float inMax= 180.0;
const float outMin= 500.0;
const float outMax= 3000.0;

void setServo();
void writeServo();
void servoSweep();

uint16_t servoWrite(float position);
uint16_t servoWriteMap(float position, float inMin, float inMax, float outMin, float outMax);
void delay(uint8_t delay);

volatile uint8_t timerOverflow= 0;
bool delayTrigger= false;

ISR(TIMER1_OVF_vect){
	timerOverflow++;
	delayTrigger== false ? (delayTrigger= true) : (delayTrigger= false); 
}

int main(void)
{
    setServo(); //Set the output frequency to ~ 50 Hz.
	
    while (1) 
    {
		//writeServo();
		servoSweep();
    }
}

void setServo(){
	//Configure Timer 1 for phase correct PWM @ 50 Hz with ICR TOP and clear on OCR match.
	DDRB|= 0x06;	//Set PWM output pins.
	TCCR1A= ((1<< COM1A1) | (1<< COM1B1) | (1<< WGM11));	//Set non-inverted PWM on channel A and B and waveform generation mode to phase correct PWM waveform (mode 10).
	TCCR1B= ((1<< WGM13) | (0<< CS12) | (1<< CS11));	//Set waveform generation mode and pre-scaler.
	TIMSK1= 0x01;	//Enable overflow interrupt (not necessary).
	ICR1= 20000;	//TOP (49.90 Hz).
	sei();	//	Enable global interrupts (not necessary). 
	TCNT1= 0;	//Initialize timer.
}

void writeServo(){
	//Set duty cycle.
	OCR1A= servoWriteMap(90, 0, 180, 500, 3000);	//Test LED.
	OCR1B= servoWriteMap(90, 0, 180, 500, 3000);	//Min- 500 (87.5 mV, 2.5%), Max- 3000 (502 mV, 15%).
}

void servoSweep(){
	//Sweeps the servo arm a 180 degrees back and forth.
	uint16_t value= 0;
	
	for(int i= 0; i<= 180; i++){
		value= servoWrite(i);
		//value= servoWriteMap(step, 0, 180, 500, 3000);
		OCR1A= 	value;	//Test LED.
		OCR1B= value;
		//delay(2);
		_delay_ms(DELAY);
	}
	for(int i= 180; i>= 0; i--){
		value= servoWrite(i);
		//value= servoWriteMap(step, 0, 180, 500, 3000);
		OCR1A= 	value;	//Test LED.
		OCR1B= value;
		//delay(2);
		_delay_ms(DELAY);
	}
}

uint16_t servoWrite(float position){
	return (uint16_t)((position- inMin)* (outMax- outMin)/ (inMax- inMin)+ outMin);
}

uint16_t servoWriteMap(float position, float inMin, float inMax, float outMin, float outMax){
	return (uint16_t)((position- inMin)* (outMax- outMin)/ (inMax- inMin)+ outMin);
}

void delay(uint8_t delay){
	/*if(timerOverflow== delay){
		delayTrigger= true;
	}
	while(delayTrigger== true){
		if(timerOverflow> delay){
			delayTrigger= false;
			timerOverflow= 0;
		}
	}*/
	
	while(delayTrigger== false){}
}

