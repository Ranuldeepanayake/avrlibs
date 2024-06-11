/*
 * shiftregister.h
 *
 * Created: 04-Feb-19 12:28:47 PM
 * Author: Ranul Deepanayake
 * 74HC595 SIPO shift register library for the ATmega328P. 
 * Uses serial bit banging. Timing is set for running at 4.5V- 6V (328P @16MHz 1 clock cycle-> 62.5 nS). Most 74CH595 timings require an average minimum of 18 nS.
 * Running below 4.5V may require delay functions which haven't been implemented.   
 * Can easily daisy chain multiple shift registers. Send LSB byte first when daisy chaining.
 * Supports selectable ports and pins. Change ports and pins according to your implementation.
 * Serial bit numbers and parallel output pin numbers are inverted (default). MCU bit 0-> Shift Register bit 7.
 */ 

#ifndef SHIFTREGISTER_H_
#define SHIFTREGISTER_H_

//Includes.
#include <avr/io.h>

//Defines.
#define SHIFT_REGISTER_DDR_REGISTER DDRB
#define SHIFT_REGISTER_PORT_REGISTER PORTB

#define SHIFT_REGISTER_DATA_CLOCK_PIN 0x02
#define SHIFT_REGISTER_DATA_OUT_PIN 0x04
#define SHIFT_REGISTER_MASTER_RECLEAR_PIN 0x08
#define SHIFT_REGISTER_LATCH_PIN 0x10
#define SHIFT_REGISTER_OUTPUT_ENABLE_PIN 0x20

#define SHIFT_REGISTER_DATA_CLOCK_PIN_HIGH (SHIFT_REGISTER_PORT_REGISTER|= SHIFT_REGISTER_DATA_CLOCK_PIN);
#define SHIFT_REGISTER_DATA_CLOCK_PIN_LOW (SHIFT_REGISTER_PORT_REGISTER&= ~SHIFT_REGISTER_DATA_CLOCK_PIN);
#define SHIFT_REGISTER_DATA_OUT_PIN_HIGH (SHIFT_REGISTER_PORT_REGISTER|= SHIFT_REGISTER_DATA_OUT_PIN);
#define SHIFT_REGISTER_DATA_OUT_PIN_LOW (SHIFT_REGISTER_PORT_REGISTER&= ~SHIFT_REGISTER_DATA_OUT_PIN);
#define SHIFT_REGISTER_MASTER_RECLEAR_PIN_HIGH (SHIFT_REGISTER_PORT_REGISTER|= SHIFT_REGISTER_MASTER_RECLEAR_PIN);
#define SHIFT_REGISTER_MASTER_RECLEAR_PIN_LOW (SHIFT_REGISTER_PORT_REGISTER&= ~SHIFT_REGISTER_MASTER_RECLEAR_PIN);
#define SHIFT_REGISTER_LATCH_PIN_HIGH (SHIFT_REGISTER_PORT_REGISTER|= SHIFT_REGISTER_LATCH_PIN);
#define SHIFT_REGISTER_LATCH_PIN_LOW (SHIFT_REGISTER_PORT_REGISTER&= ~SHIFT_REGISTER_LATCH_PIN);
#define SHIFT_REGISTER_OUTPUT_ENABLE_PIN_HIGH (SHIFT_REGISTER_PORT_REGISTER|= SHIFT_REGISTER_OUTPUT_ENABLE_PIN);
#define SHIFT_REGISTER_OUTPUT_ENABLE_PIN_LOW (SHIFT_REGISTER_PORT_REGISTER&= ~SHIFT_REGISTER_OUTPUT_ENABLE_PIN);

//Functions.
//Set up the shift register.
void shift_register_set();
//Send one byte to the shift register. MCU bit 0-> Shift Register bit 7. Send LSB byte first when daisy chaining.
void shift_register_send_byte(uint8_t byte);
//Latch contents of the shift register buffer to shift register memory.
void shift_register_latch();
//Reset shift register latch signal.
void shift_register_delatch();
//Enable parallel output pins of the shift register.
void shift_register_enable_output();
//Disable parallel output pins of the shift register.
void shift_register_disable_output();
//Reset shift register buffer clear signal.
void shift_register_serial_set();
//Clear contents of the shift register buffer.
void shift_register_serial_clear();

/*
Example implementation.

#include "timer.h"

int main(void)
{
	shift_register_set();
	shift_register_serial_set();
	
	while (1)
	{
		shift_register_disable_output();
		shift_register_delatch();
		shift_register_send_byte(0xFF);	//Send byte to second shift register (LSB byte first).
		shift_register_send_byte(0x81); //Send byte to first shift register.
		shift_register_latch();
		shift_register_enable_output();
	}
}
*/

#endif /* SHIFTREGISTER_H_ */