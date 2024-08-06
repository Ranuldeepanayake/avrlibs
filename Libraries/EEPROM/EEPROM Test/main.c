/*
 * Internal EEPROM.c
 *
 * Created: 11-Dec-18 11:05:59 AM
 * Author : Ranul Deepanayake
 * Demonstrates the use of the internal EEPROM.
 * Note that, to write to a location in EEPROM, the respective location must be erased first.
 * The EEPROM gets overwritten when being flashed if the EEPROM preserve fuse is not set.
 */ 

#include <avr/io.h>
#include "eeprom.h"
#include "uart.h"

int main(void)
{	
	char data[8] = {0x00, 0x10, 0x44, 0x23, 0x64, 0x86, 0x49, 0x50};
	uint16_t *crc = 0;
	
	uartSet(UART_BAUD_RATE(115200), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);
	//eepromWrite(0x00, 8, data, crc);
	//eepromRead(0x00, 8, data, crc);
	
	//eepromWrite(0x0002, 1, &data[4], crc);
	eepromRead(0x0000, 1, &data[3], crc);
	eepromRead(0x0002, 1, &data[4], crc);
	
    while (1) 
    {
    }
}


