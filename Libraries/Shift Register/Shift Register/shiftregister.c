/*
 * shiftregister.c
 *
 * Created: 04-Feb-19 12:28:29 PM
 * Author: Ranul Deepanayake
 */ 

#include "shiftregister.h"

/*
Set up the shift register.
*/
void shift_register_set(){
	SHIFT_REGISTER_DDR_REGISTER|= (SHIFT_REGISTER_DATA_CLOCK_PIN | SHIFT_REGISTER_DATA_OUT_PIN | SHIFT_REGISTER_MASTER_RECLEAR_PIN |
	SHIFT_REGISTER_LATCH_PIN | SHIFT_REGISTER_OUTPUT_ENABLE_PIN);
}

/*
Send one byte to the shift register. 
MCU bit 0-> Shift Register bit 7.
Send LSB byte first when daisy chaining.
*/
void shift_register_send_byte(uint8_t byte){
	//LSB first. MCU Bit 0-> Shift Register Bit 0.
	uint8_t mask= 1;
	for(uint8_t i= 1; i<= 8; i++){
		
		if(byte & mask){
			SHIFT_REGISTER_DATA_OUT_PIN_HIGH
		}else{
			SHIFT_REGISTER_DATA_OUT_PIN_LOW
		}
		
		SHIFT_REGISTER_DATA_CLOCK_PIN_HIGH
		SHIFT_REGISTER_DATA_OUT_PIN_LOW
		SHIFT_REGISTER_DATA_CLOCK_PIN_LOW
		
		mask*= 2;
	}
}

/*
Latch contents of the shift register buffer to shift register memory.
*/
void shift_register_latch(){
	SHIFT_REGISTER_LATCH_PIN_HIGH
}

/*
Reset shift register latch signal.
*/
void shift_register_delatch(){
	SHIFT_REGISTER_LATCH_PIN_LOW
}

/*
Enable parallel output pins of the shift register.
*/
void shift_register_enable_output(){
	SHIFT_REGISTER_OUTPUT_ENABLE_PIN_LOW
}

/*
Disable parallel output pins of the shift register. 
*/
void shift_register_disable_output(){
	SHIFT_REGISTER_OUTPUT_ENABLE_PIN_HIGH
}

/*
Reset shift register buffer clear signal. 
*/
void shift_register_serial_set(){
	SHIFT_REGISTER_MASTER_RECLEAR_PIN_HIGH
}

/*
Clear contents of the shift register buffer.
*/
void shift_register_serial_clear(){
	SHIFT_REGISTER_MASTER_RECLEAR_PIN_LOW
}

