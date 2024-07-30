/*
 * eeprom.h
 *
 * Created: 29-Jul-24 7:32:52 PM
 * Author: Internal EEPROM library for the ATmega 328P.
 * Supports reads/writes on the byte and block level.
 * ***Supports CRC calculation for block reads and writes (under development).
 * ***Need to implement a watchdog timer.
 * Supports debugging.
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

//Debug levels.
#define EEPROM_DEBUG_LEVEL_0 0
#define EEPROM_DEBUG_LEVEL_1 1
#define EEPROM_DEBUG_LEVEL_2 2
//The debug switch.
#define EEPROM_DEBUG_LEVEL EEPROM_DEBUG_LEVEL_0
//Address range definition.
#if mcu == atmega328p
	#define EEPROM_SIZE 1024
	#define EEPROM_ADDRESS_LOW 0
	#define EEPROM_ADDRESS_HIGH 1023
#endif
//Modes.
#define EEPROM_MODE_ERASE_AND_WRITE 0x00
#define EEPROM_MODE_ERASE 0x10
#define EEPROM_MODE_WRITE 0x20
#define EEPROM_READ_ENABLE 0x01
#define EEPROM_WRITE_ENABLE 0x02
#define EEPROM_WRITE_MASTER_ENABLE 0x04
//Status codes.
#define EEPROM_SUCCESS 1
#define EEPROM_CAPACITY_OVERFLOW 2
#define EEPROM_ADDRESS_RANGE_ERROR 3
#define EEPROM_WRITE_PENDING 4

/*
Functions.
*/
//Write a block of data to the EEPROM.
uint8_t eepromWrite(uint16_t start_address, uint16_t bytes, char *data, uint16_t *crc);
//Write a byte of data to the EEPROM.
uint8_t eepromWriteByte(uint16_t address, char data);
//Read a block of data from the EEPROM.
uint8_t eepromRead(uint16_t start_address, uint16_t bytes, char *data, uint16_t *crc);
//Read a byte of data from the EEPROM.
uint8_t eepromReadByte(uint16_t address, char *data);
#if EEPROM_DEBUG_LEVEL > 0
//Supporting function for debugging.
void eepromDebugDecToHex(uint16_t input, char *temp_1);
#endif

/*

Example implementation.

int main(void)
{
	char data[8] = {0x00, 0x10, 0x44, 0x23, 0x64, 0x86, 0x49, 0x50};
	uint16_t *crc = 0;
	
	uartSet(UART_BAUD_RATE(115200), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);
	eepromWrite(0x00, 8, data, crc);
	eepromRead(0x00, 8, data, crc);
	
	while (1)
	{
	}
}

*/

#endif /* EEPROM_H_ */