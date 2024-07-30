/*
 * eeprom.h
 *
 * Created: 29-Jul-24 7:32:52 PM
 * Author: Internal EEPROM library for the ATmega 328P.
 * Supports reads/writes on the byte and block level.
 * Supports CRC calculation for block reads and writes (under development).
 * Supports debugging.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/crc16.h>
#include "eeprom.h"

#if EEPROM_DEBUG_LEVEL > 0
	#include <string.h>
	#include <stdlib.h>
	#include "uart.h"
#endif 

/*
Writes a block of data to the EEPROM starting at the specified address. Calculates the expected CRC for the data written (under development).
The EEPROM word size is 1 byte and the data source must fragment larger word sizes appropriately. 
*/
uint8_t eepromWrite(uint16_t start_address, uint16_t bytes, char *data, uint16_t *crc){
	if(bytes >= EEPROM_SIZE){
		return EEPROM_CAPACITY_OVERFLOW;
	}
	if((start_address < EEPROM_ADDRESS_LOW) || (start_address > EEPROM_ADDRESS_HIGH)){
		return EEPROM_ADDRESS_RANGE_ERROR;
	}
	if((start_address + bytes) > EEPROM_SIZE){
		return EEPROM_CAPACITY_OVERFLOW;
	}
	
	*crc = 0;
	char data_temp = 0;
	
	#if EEPROM_DEBUG_LEVEL >= 1
		char temp[7];
		uartPrintLn("[EEPROM][eepromWrite][Writing]");
	#endif
	
	for(uint16_t i = 0; i < bytes; i++){
		data_temp = *(data + i);
		crc = (uint16_t*) _crc16_update(*crc, data_temp);
		eepromWriteByte(start_address++,  data_temp); //Increment the EEPROM address by one.
		
		#if EEPROM_DEBUG_LEVEL >= 1
		//Print a new line every 16 bytes.
		if((i != 0) && (i % 16 == 0)){
			uartPrintLn("");
		}
			
		eepromDebugDecToHex((uint8_t) data_temp, temp);
		uartPrint(temp); uartPrint(" ");
		#endif	
	}
	
	#if EEPROM_DEBUG_LEVEL >= 1
		uartPrintLn("");
	#endif
	#if EEPROM_DEBUG_LEVEL >= 2
		eepromDebugDecToHex((uint8_t) bytes, temp);
		uartPrint("[EEPROM][eepromWrite][Complete]["); uartPrint(temp); uartPrint(" : bytes written][CRC : ");
		eepromDebugDecToHex((uint16_t) *crc, temp);
		uartPrint(temp); uartPrintLn("]");
		
	#endif
	
	return EEPROM_SUCCESS;
}

/*
Writes a byte of data to the EEPROM starting at the specified address.
The EEPROM word size is 1 byte and the data source must fragment larger word sizes appropriately.
*/
uint8_t eepromWriteByte(uint16_t address, char data){
	//Only effective if this function is called directly. Block writes already have protection.
	if((address < EEPROM_ADDRESS_LOW) || (address > EEPROM_ADDRESS_HIGH)){
		return EEPROM_ADDRESS_RANGE_ERROR;
	}
	
	//If there is a pending EEPROM write, no other EEPROM operation can be performed.
	while(EECR & EEPROM_WRITE_ENABLE);
	
	//If there is a pending EEPROM write, no other EEPROM operation can be performed.
	while(EECR & EEPROM_WRITE_MASTER_ENABLE);

	EECR = EEPROM_MODE_ERASE_AND_WRITE;	//Clear the EEPROM control register and set the mode.
	
	cli();		//Disable global interrupts to prevent disrupting the the operation.
	EEARH = address >> address;		//Set up the EEPROM address registers.
	EEARL = address;
	EEDR = data;		//Write data to the EEPROM data register.
	EECR |= EEPROM_WRITE_MASTER_ENABLE;	//Set EEPROM master write enable. The next action must be taken within four cycles before this flag gets cleared by hardware.
	EECR |= EEPROM_WRITE_ENABLE;	//Write to EEPROM.
	
	while(EECR & EEPROM_WRITE_ENABLE);		//Wait for the write operation to finish.
	sei();		//Re-enable global interrupts.
	
	return EEPROM_SUCCESS;
}

/*
Reads a block of data from the EEPROM starting at the specified address. Calculates the actual CRC for the data read (under development).
The EEPROM word size is 1 byte and the data source must fragment larger word sizes appropriately.
*/
uint8_t eepromRead(uint16_t start_address, uint16_t bytes, char *data, uint16_t *crc){
	if(bytes >= EEPROM_SIZE){
		return EEPROM_CAPACITY_OVERFLOW;
	}
	if((start_address < EEPROM_ADDRESS_LOW) || (start_address > EEPROM_ADDRESS_HIGH)){
		return EEPROM_ADDRESS_RANGE_ERROR;
	}
	if((start_address + bytes) > EEPROM_SIZE){
		return EEPROM_CAPACITY_OVERFLOW;
	}
	
	*crc = 0;
	char *data_temp = 0;
	
	#if EEPROM_DEBUG_LEVEL >= 1
		char temp[7];
		uartPrintLn("[EEPROM][eepromRead][Reading]");
	#endif
	
	for(uint16_t i = 0; i < bytes; i++){
		eepromReadByte(start_address++, data_temp); //Increment the EEPROM address by one
		*(data + i) = *data_temp;
		//crc = (uint16_t*) _crc16_update(*crc, (uint8_t) *data_temp); //***causes bad output.
		
		#if EEPROM_DEBUG_LEVEL >= 1
			//Print a new line every 16 bytes.
			if((i != 0) && (i % 16 == 0)){
				uartPrintLn("");
			}
			
			eepromDebugDecToHex((uint8_t) *data_temp, temp);
			uartPrint(temp); uartPrint(" ");
		#endif
	}
	
	#if EEPROM_DEBUG_LEVEL >= 1
		uartPrintLn("");
	#endif
	#if EEPROM_DEBUG_LEVEL >= 2
		eepromDebugDecToHex((uint8_t) bytes, temp);
		uartPrint("[EEPROM][eepromWrite][Complete]["); uartPrint(temp); uartPrint(" : bytes written][CRC : ");
		eepromDebugDecToHex((uint16_t) *crc, temp);
		uartPrint(temp); uartPrintLn("]");
	
	#endif
	
	return EEPROM_SUCCESS;
}

/*
Reads a byte of data from the EEPROM starting at the specified address.
The EEPROM word size is 1 byte and the data source must fragment larger word sizes appropriately.
*/
uint8_t eepromReadByte(uint16_t address, char *data){
	//Only effective if this function is called directly. Block reads already have protection.
	if((address < EEPROM_ADDRESS_LOW) || (address > EEPROM_ADDRESS_HIGH)){
		return EEPROM_ADDRESS_RANGE_ERROR;
	}
	
	//If there is a pending EEPROM write, no other EEPROM operation can be performed.
	while(EECR & EEPROM_WRITE_ENABLE);

	EECR = EEPROM_MODE_ERASE_AND_WRITE;	//Clear the EEPROM control register and set the mode.
	
	cli();		//Disable global interrupts to prevent disrupting the the operation.
	EEARH = address >> address;		//Set up the EEPROM address registers.
	EEARL = address;
	EECR |= EEPROM_READ_ENABLE;	//Read from EEPROM. Set this bit to '1' to execute the read process.
		
	while(EECR & EEPROM_READ_ENABLE);		//Wait for the read operation to finish.
	*data = EEDR;		//Read one byte of data.
	sei();		//Re-enable global interrupts.

	return EEPROM_SUCCESS;
}

void eepromErase(){
	
}

#if EEPROM_DEBUG_LEVEL > 0
/*
Internal function to convert integers from the decimal format to hexadecimal.
*/
void eepromDebugDecToHex(uint16_t input, char *temp_1){
	char temp_2[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	memset(temp_1, 0, sizeof(&temp_1));
	
	itoa(input, temp_2, 16);
	if(input < 16) {
		strcat(temp_1, "0x0");
		strcat(temp_1, temp_2);
	}else{
		strcat(temp_1, "0x");
		strcat(temp_1, temp_2);
	}
}
#endif

