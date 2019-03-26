/**
 * ramdump.h
 *
 * Created: 26-Mar-19 2:32:59 PM
 * Author: Ranul Deepanayake
 * RAM dump library for AVR microcontrollers. Supports dumping over UART. **Only works with optimization level 0.
 * Supports configurable memory address ranges. Supports a test mode with a test data set.
 * Please change the required defines according to your usage.
 * Requires the UART library.
 */ 


#ifndef RAMDUMP_H_
#define RAMDUMP_H_

//Includes.
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"

//Defines.
#define RAM_DUMP_TEST_MODE	//Uncomment to enable test mode. Will fill RAM with a pattern.
#define RAM_DUMP_READ_START_LOCATION 0x0000	//Change according to the microcontroller.
#define RAM_DUMP_READ_STOP_LOCATION 0x08FF	//Change according to the microcontroller.
#define RAM_DUMP_DATA_SET_SIZE 128

//Functions.
void ramdump_dump_serial(void);

//External variables.
#ifdef RAM_DUMP_TEST_MODE
extern uint8_t ram_dump_test_data[RAM_DUMP_DATA_SET_SIZE];	//Test data set.
#endif

/**
Example implementation.

int main(void)
{
	uart_set(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);
	ramdump_dump_serial();
	
	while (1);
}
*/
#endif /* RAMDUMP_H_ */