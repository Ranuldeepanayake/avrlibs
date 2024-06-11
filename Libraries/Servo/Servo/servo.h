/*
 * Servo Control.c
 *
 * Created: 13-Oct-18 8:03:11 PM
 * Author : Ranul Deepanayake
 * Servo motor library for the ATmega328P using the SG90 micro-servo.
 * Timer 1 and output compare A/B are used. Can control only two servos.
 * Servo 1-> OCR1A, servo 2-> OCR1B.
 * Servos are controlled with a 50Hz phase correct 1mS- 2mS duty cycle square wave.
 * Initializes servo to 90 degrees.
 * OCR values tested-> Min- 500 (87.5 mV, 2.5%), Max- 3000 (502 mV, 15%).
 * Minimum and maximum OCR values may have to be changed depending on the servo. 
 */ 


#ifndef SERVO_H_
#define SERVO_H_

//Includes.
#include <avr/io.h>
#include <util/delay.h>

//Defines.
#define SERVO_DDR_REGISTER DDRB
#define SERVO_TCCRA_REGISTER TCCR1A
#define SERVO_TCCRB_REGISTER TCCR1B
#define SERVO_ICR_REGISTER ICR1
#define SERVO_TCNT_REGISTER TCNT1
#define SERVO_0_PIN 0x02
#define SERVO_1_PIN 0x04

#define SERVO_0 0
#define SERVO_1 1
#define SERVO_0_COM_MODE 0x80
#define SERVO_1_COM_MODE 0x20
#define SERVO_0_OCR OCR1A
#define SERVO_1_OCR OCR1B 
#define SERVO_WAVEFORM_MODE_10 (SERVO_TCCRA_REGISTER|= 0x02); (SERVO_TCCRB_REGISTER|= 0x10);
#define SERVO_TIMER_PRESCALER 0x02

#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 179
#define SERVO_SWEEP_DELAY 5
#define SERVO_MIN_OCR 500  //400. Change accordingly.
#define SERVO_MAX_OCR 3000 //2600. Change accordingly.
#define SERVO_ICR_VALUE 20000 //Gives 50Hz.

//Functions.
//Set up the servo motor. Servo 1-> OCR1A, servo 2-> OCR1B. 
void servo_set(uint8_t servo_number);
//Actuate the specified servo according to the specified angular position (0- 180 degrees).
void servo_write(uint8_t servo_number, uint8_t angle);
//Sweeps connected and initialized servos 180 degrees and back.
void servo_sweep();
//Map angular position to OCR value.
uint16_t servo_map(float position);

/*
Example implementation.

#include "servo.h"

int main(void)
{
	servo_set(SERVO_0);	//Servo on OCR1A.
	servo_set(SERVO_1); //Servo on OCR1B.
	
	while (1)
	{
		servo_write(SERVO_0, 45); //Move servo 45 degrees.
	}
}
*/

#endif /* SERVO_H_ */