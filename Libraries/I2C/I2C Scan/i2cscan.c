/*
 * I2C Scan.c
 *
 * Created: 24-Jul-24 11:11:28 AM
 * Author: Ranul Deepanayake
 * I2C bus scanner library for AVR microcontrollers.
 * Scans addresses from 0-127 with the write mode.
 */

#include <avr/io.h>
#include <string.h> //For the scanner function.
#include <stdlib.h> //For the scanner function.
#include "i2cscan.h"
#include "i2c.h"
#include "uart.h"

/*
Scans I2C addresses from 0-127 using the write mode. Checks if bus hangup detection is present or not.
Multiple devices may respond to the general cal address '0x00'. 
*/
void i2cScan(){
	char temp[5];
	
	#ifdef BUS_HANGUP_PROTECTION_ENABLED
	uartPrintLn("[i2cscan][BUS hangup protection enabled]");
	#else
	uartPrintLn("[i2cscan][BUS hangup protection disabled]");
	#endif
	
	uartPrintLn("[i2cscan][Starting scan]");
	
	for(uint8_t address = 0; address <= 127; address++){
		//If a slave responds.
		if((address != 0) && (i2cDelayedStart(address, I2C_WRITE) == SUCCESS)){
			i2cScanDebugIntToHex(address, temp); //Gives the I2C address, not the byte address.
			uartPrint("[i2cscan][Device responded at address] : "); uartPrintLn(temp);
			
		}else if((address == 0) && (i2cDelayedStart(address, I2C_WRITE) == SUCCESS)){
			uartPrintLn("[i2cscan][Device(s) responded to general call address 0x00]");
		}
	}
	
	uartPrintLn("[i2cscan][Scan complete]");
}

/*
Supporting function which converts numbers in the decimal format to hexadecimal format when storing as a string. 
*/
void i2cScanDebugIntToHex(uint16_t input, char *temp_1){
	char temp_2[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	memset(temp_1, 0, sizeof(&temp_1));
	//memset(temp2, 0, STRING_LENGTH);
	//sprintf(temp2, "%x", input);
	itoa(input, temp_2, 16);
	if(input < 16) {
		strcat(temp_1, "0x0");
		strcat(temp_1, temp_2);
		//uartPrint("0x0"); uartPrint(temp2);
	}else{
		strcat(temp_1, "0x");
		strcat(temp_1, temp_2);
		//uartPrint("0x"); uartPrint(temp2);
	}
}