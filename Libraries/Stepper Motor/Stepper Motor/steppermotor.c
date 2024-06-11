/*
 * steppermotor.c
 *
 * Created: 02-Feb-19 7:42:03 PM
 * Author: Ranul Deepanayake
 */ 

#include "steppermotor.h"

/*
Set up the stepper motor.
*/
void stepper_motor_set(){
	STEPPER_MOTOR_DDR_REGISTER|= (STEPPER_MOTOR_INPUT_A_PIN | STEPPER_MOTOR_INPUT_B_PIN | STEPPER_MOTOR_INPUT_C_PIN | STEPPER_MOTOR_INPUT_D_PIN);
}

void stepper_motor_step(uint8_t direction, uint16_t value, uint8_t unit, uint8_t speed, uint8_t mode){
	//Unit conversion.
	if(unit== STEPPER_MOTOR_UNIT_ANGLE && (mode== STEPPER_MOTOR_MODE_FULL_STEP || mode== STEPPER_MOTOR_MODE_WAVE_STEP)){
		uint32_t angle_to_steps= (value* STEPPER_MOTOR_FULL_STEP_STEPS_PER_DEGREE)/10000; //10000 div factor (0.0889-> 889). Had to be promoted to UL for the compiler to understand. Decimal error present.
		value= (uint8_t) angle_to_steps;			   // 1 step/11.25 degrees= 0.0089.
		
	}else if(unit== STEPPER_MOTOR_UNIT_ANGLE && mode== STEPPER_MOTOR_MODE_HALF_STEP){
		uint32_t angle_to_steps= (value* STEPPER_MOTOR_HALF_STEP_STEPS_PER_DEGREE)/10000; //10000 div factor (0.1778-> 1778). Had to be promoted to UL for the compiler to understand. Decimal error present.s
		value= (uint8_t) angle_to_steps;				// 1 step/5.625 degrees= 0.1778.
		
	}else if(unit== STEPPER_MOTOR_UNIT_STEPS){
		value= value;
	}
	
	//Handle bad input.
	if((mode== STEPPER_MOTOR_MODE_FULL_STEP || mode== STEPPER_MOTOR_MODE_WAVE_STEP) && (value< 0 || value> 32)){
		value= 1;
	}else if(mode== STEPPER_MOTOR_MODE_HALF_STEP && (value< 0 || value> 64)){
		value= 1;
	}
	
	//Full step mode. Divide value by two get motor steps.
	if(mode== STEPPER_MOTOR_MODE_FULL_STEP){
		for(uint16_t i= 0; i< (value* STEPPER_MOTOR_GEAR_RATIO)/STEPPER_MOTOR_FULL_STEP_COUNT_INTERNAL; i++){
			if(direction== STEPPER_MOTOR_DIRECTION_FORWARD){
				stepper_motor_full_step_forward(speed);
			}else if(direction== STEPPER_MOTOR_DIRECTION_REVERSE){
				stepper_motor_full_step_reverse(speed);
			}
		}
		
		//Half step mode.
		}else if(mode== STEPPER_MOTOR_MODE_HALF_STEP){
		for(uint16_t i= 0; i< (value* STEPPER_MOTOR_GEAR_RATIO)/STEPPER_MOTOR_HALF_STEP_COUNT_INTERNAL; i++){
			if(direction== STEPPER_MOTOR_DIRECTION_FORWARD){
				stepper_motor_half_step_forward(speed);
				}else if(direction== STEPPER_MOTOR_DIRECTION_REVERSE){
				stepper_motor_half_step_reverse(speed);
			}
		}
	}
	
	//Wave step mode. Divide value by two get motor steps.
	else if(mode== STEPPER_MOTOR_MODE_WAVE_STEP){
		for(uint16_t i= 0; i< (value* STEPPER_MOTOR_GEAR_RATIO)/STEPPER_MOTOR_WAVE_STEP_COUNT_INTERNAL; i++){
			if(direction== STEPPER_MOTOR_DIRECTION_FORWARD){
				stepper_motor_wave_drive_forward(speed);
				}else if(direction== STEPPER_MOTOR_DIRECTION_REVERSE){
				stepper_motor_wave_drive_reverse(speed);
			}
		}
	}
}



/*
Full step drive forward (2 coils are active at a time).
Function drives 4 motor steps.
1 360 degree shaft rotation-> 4 seconds. Provides more torque but reduced resolution.
Provides less resolution than the half step mode.
Tested with a 2ms interval @ 12V DC.
*/
void stepper_motor_full_step_forward(uint8_t step_delay){
	if(1){
		STEPPER_MOTOR_INPUT_A_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_D_LOW;
		STEPPER_MOTOR_INPUT_B_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_A_LOW
		STEPPER_MOTOR_INPUT_C_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_B_LOW
		STEPPER_MOTOR_INPUT_D_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_C_LOW
	}
	STEPPER_MOTOR_INPUT_D_LOW
}

/*
Reverse of the preceding.
*/
void stepper_motor_full_step_reverse(uint8_t step_delay){
	if(1){
		STEPPER_MOTOR_INPUT_A_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_B_LOW;
		STEPPER_MOTOR_INPUT_D_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_A_LOW
		STEPPER_MOTOR_INPUT_C_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_D_LOW
		STEPPER_MOTOR_INPUT_B_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_C_LOW
	}
	STEPPER_MOTOR_INPUT_B_LOW
}

/*
Half step drive forward (2 coils are active at a time).
Function drives 8 motor steps.
1 360 degree shaft rotation-> 4 seconds. Provides better torque and resolution.
Provides less resolution than the half step mode.
Tested with a 2ms interval @ 12V DC.
*/
void stepper_motor_half_step_forward(uint8_t step_delay){
	if(1){
		STEPPER_MOTOR_INPUT_A_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_D_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_B_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_A_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_C_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_B_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_D_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_C_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	}
	STEPPER_MOTOR_INPUT_D_LOW
}

/*
Reverse of the preceding.
*/
void stepper_motor_half_step_reverse(uint8_t step_delay){
	if(1){
		STEPPER_MOTOR_INPUT_A_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_B_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_D_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_A_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_C_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_D_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
		STEPPER_MOTOR_INPUT_B_HIGH
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		STEPPER_MOTOR_INPUT_C_LOW
		_delay_ms(STEPPER_MOTOR_STEP_DELAY);
		
	}
	STEPPER_MOTOR_INPUT_B_LOW
}

/*
Wave drive forward (single coil excitation: 4 step drive where only one coil is active at a time).
Function drives 4 motor steps.
1 360 degree shaft rotation-> 8 seconds. Provides more torque but reduced resolution.
Tested with with 2ms step interval @ 12V DC.
*/
void stepper_motor_wave_drive_forward(uint8_t step_delay){
	STEPPER_MOTOR_INPUT_A_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_A_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_B_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_B_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_C_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_C_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_D_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_D_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
}

/*
Reverse of the preceding.
*/
void stepper_motor_wave_drive_reverse(uint8_t step_delay){
	STEPPER_MOTOR_INPUT_A_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_A_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_D_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_D_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_C_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_C_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	
	STEPPER_MOTOR_INPUT_B_HIGH
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
	STEPPER_MOTOR_INPUT_B_LOW
	_delay_ms(STEPPER_MOTOR_STEP_DELAY);
}
