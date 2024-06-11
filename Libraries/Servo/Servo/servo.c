/*
 * servo.c
 *
 * Created: 05-Feb-19 5:34:07 PM
 * Author: Ranul Deepanayake
 */ 

#include "servo.h"

/*
Set up the servo motor.
Timer 1 and output compare A/B are used. Can control only two servos.
Servo 1-> OCR1A, servo 2-> OCR1B. 
Servos are controlled with a 50Hz phase correct 1mS- 2mS duty cycle square wave.
Initializes servo to 90 degrees.
*/
void servo_set(uint8_t servo_number){
	SERVO_WAVEFORM_MODE_10	//Set waveform generation mode to phase correct PWM (mode 10).
	SERVO_TCCRB_REGISTER|= SERVO_TIMER_PRESCALER;	//Set prescaler to 8.
	SERVO_ICR_REGISTER= SERVO_ICR_VALUE;	//TOP (gives 49.90 Hz). Sets frequency.
	SERVO_TCNT_REGISTER= 0;	//Initialize timer counter.
	
	switch(servo_number){
		case SERVO_0:
		SERVO_TCCRA_REGISTER|= SERVO_0_COM_MODE;	//Set non-inverted PWM on channel A.
		SERVO_DDR_REGISTER|= SERVO_0_PIN;	//Set pin as output. Required.
		servo_write(SERVO_0, 90);			//Initialize position.
		break;
		case SERVO_1:
		SERVO_TCCRA_REGISTER|= SERVO_1_COM_MODE;
		SERVO_DDR_REGISTER|= SERVO_1_PIN;
		servo_write(SERVO_1, 90);
		break;
		default:
		SERVO_DDR_REGISTER|= SERVO_0_PIN;
		servo_write(SERVO_0, 90);
		break;
	}
}

/*
Actuate the specified servo according to the specified angular position (0- 180 degrees).
*/
void servo_write(uint8_t servo_number, uint8_t angle){
	if(angle< SERVO_MIN_ANGLE || angle> SERVO_MAX_ANGLE){
		angle= 90;
	}
	
	OCR1B= servo_map(angle);	
	switch(servo_number){
		case SERVO_0:
		SERVO_0_OCR= servo_map(angle);
		break;
		case SERVO_1:
		SERVO_1_OCR= servo_map(angle);
		break;
		default:
		SERVO_0_OCR= servo_map(angle);
		break;
	}
}

/*
Sweeps connected and initialized servos 180 degrees and back.
Both servos have to be set up first.
*/
void servo_sweep(){
	for(int i= 0; i<= 180; i++){
		servo_write(SERVO_0, i);
		servo_write(SERVO_1, i);
		_delay_ms(SERVO_SWEEP_DELAY);
	}
	for(int i= 180; i>= 0; i--){
		servo_write(SERVO_0, i);
		servo_write(SERVO_1, i);
		_delay_ms(SERVO_SWEEP_DELAY);
	}
}

/*
Map angular position to OCR value.
*/
uint16_t servo_map(float position){
	return (uint16_t)((position- SERVO_MIN_ANGLE)* (SERVO_MAX_OCR- SERVO_MIN_OCR)/ (SERVO_MAX_ANGLE- SERVO_MIN_ANGLE)+ SERVO_MIN_OCR);
}
