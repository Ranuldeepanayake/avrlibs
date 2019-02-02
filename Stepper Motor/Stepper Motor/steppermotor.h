/*
 * steppermotor.h
 *
 * Created: 02-Feb-19 7:41:46 PM
 * Author: Ranul Deepanayake
 * Stepper motor driver for the ATmega328P using the 28-BYJ48 stepper motor+ ULN2003 controller.
 * Supports configurable ports and pins.
 * Supports full step, half step and wave step modes.
 * Supports direction control. **Speed control is limited to a constant for now. 
 * Step units are from 1- 32 (steps) and 1- 360 (degrees) for full step and wave step and 1- 64 (steps) and 1- 360 (degrees) for half step.
 *
 * Full step drive- 32 motor steps and 11.25 degrees/step. 11.25* 32= 360 degrees (one shaft rotation).
 * Half step drive- 64 motor steps and 5.625 degrees/step. 5.625* 64= 360 degrees (one shaft rotation). Has the best resolution and good torque.
 * Wave drive- 32 motor steps and 11.25 degrees/step. 11.25* 32= 360 degrees (one shaft rotation). Very low torque.
 *
 * Gear ratio 1:64.
 * The following benchmarks are for the full step mode, 12V DC and 2ms step interval (speed setting 2).
 * Max RPM: 15.
 * Time for 1 rotation at 15RPM: 4s (60/15).
 * Min. delay between gear steps: 62.5ms (4s/64 gear ratio).
 * Min. delay between motor steps: 1.9ms (62.5s/32).
 */ 


#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

//Includes.
#include <avr/io.h>
#include <util/delay.h>

//Attributes.
#define STEPPER_MOTOR_DDR_REGISTER DDRD
#define STEPPER_MOTOR_PORT_REGISTER	PORTD
#define STEPPER_MOTOR_INPUT_A_PIN 0x10
#define STEPPER_MOTOR_INPUT_B_PIN 0x20
#define STEPPER_MOTOR_INPUT_C_PIN 0x40
#define STEPPER_MOTOR_INPUT_D_PIN 0x80

#define STEPPER_MOTOR_STEPS 64
#define STEPPER_MOTOR_GEAR_RATIO 64
#define STEPPER_MOTOR_STEP_DELAY 2	//2ms- 20ms looks good.
#define STEPPER_MOTOR_MODE_FULL_STEP 0
#define STEPPER_MOTOR_MODE_HALF_STEP 1
#define STEPPER_MOTOR_MODE_WAVE_STEP 2
#define STEPPER_MOTOR_DIRECTION_FORWARD 0
#define STEPPER_MOTOR_DIRECTION_REVERSE 1
#define STEPPER_MOTOR_UNIT_STEPS 0
#define STEPPER_MOTOR_UNIT_ANGLE 1
#define STEPPER_MOTOR_FULL_STEP_COUNT_INTERNAL 4
#define STEPPER_MOTOR_HALF_STEP_COUNT_INTERNAL 8
#define STEPPER_MOTOR_WAVE_STEP_COUNT_INTERNAL 4
#define STEPPER_MOTOR_FULL_STEP_COUNT_INTERNAL 4
#define STEPPER_MOTOR_FULL_STEP_STEPS_PER_DEGREE 889UL	//Used for angle to step conversion.
#define STEPPER_MOTOR_HALF_STEP_STEPS_PER_DEGREE 1778UL	//Used for angle to step conversion.

#define STEPPER_MOTOR_INPUT_A_HIGH STEPPER_MOTOR_PORT_REGISTER|= STEPPER_MOTOR_INPUT_A_PIN;
#define STEPPER_MOTOR_INPUT_A_LOW STEPPER_MOTOR_PORT_REGISTER&= ~STEPPER_MOTOR_INPUT_A_PIN;
#define STEPPER_MOTOR_INPUT_B_HIGH STEPPER_MOTOR_PORT_REGISTER|= STEPPER_MOTOR_INPUT_B_PIN;
#define STEPPER_MOTOR_INPUT_B_LOW STEPPER_MOTOR_PORT_REGISTER&= ~STEPPER_MOTOR_INPUT_B_PIN;
#define STEPPER_MOTOR_INPUT_C_HIGH STEPPER_MOTOR_PORT_REGISTER|= STEPPER_MOTOR_INPUT_C_PIN;
#define STEPPER_MOTOR_INPUT_C_LOW STEPPER_MOTOR_PORT_REGISTER&= ~STEPPER_MOTOR_INPUT_C_PIN;
#define STEPPER_MOTOR_INPUT_D_HIGH STEPPER_MOTOR_PORT_REGISTER|= STEPPER_MOTOR_INPUT_D_PIN;
#define STEPPER_MOTOR_INPUT_D_LOW STEPPER_MOTOR_PORT_REGISTER&= ~STEPPER_MOTOR_INPUT_D_PIN;

//Functions.
//Set up the stepper motor.
void stepper_motor_set();
//Drive the stepper motor according to the specified direction, unit value, unit (steps or angle), speed *defined as a constant for now and mode.
void stepper_motor_step(uint8_t direction, uint16_t value, uint8_t unit, uint8_t speed, uint8_t mode);
//Drive 4 motor steps forward in full step mode.
void stepper_motor_full_step_forward(uint8_t step_delay);
//Drive 4 motor steps reverse in full step mode.
void stepper_motor_full_step_reverse(uint8_t step_delay);
//Drive 8 motor steps forward in half step mode.
void stepper_motor_half_step_forward(uint8_t step_delay);
//Drive 8 motor steps reverse in half step mode.
void stepper_motor_half_step_reverse(uint8_t step_delay);
//Drive 4 motor steps forward in wave drive mode.
void stepper_motor_wave_drive_forward(uint8_t step_delay);
//Drive 4 motor steps reverse in wave drive mode.
void stepper_motor_wave_drive_reverse(uint8_t step_delay);

/*
Example implementation.

#include "steppermotor.h"

int main(void)
{
	stepper_motor_set();
	
	stepper_motor_step(STEPPER_MOTOR_DIRECTION_FORWARD, 32, STEPPER_MOTOR_UNIT_STEPS, 10, STEPPER_MOTOR_MODE_FULL_STEP);
	_delay_ms(250);
	stepper_motor_step(STEPPER_MOTOR_DIRECTION_REVERSE, 360, STEPPER_MOTOR_UNIT_ANGLE, 10, STEPPER_MOTOR_MODE_FULL_STEP);
	
	while (1)
	{
	}
}
*/

#endif /* STEPPERMOTOR_H_ */