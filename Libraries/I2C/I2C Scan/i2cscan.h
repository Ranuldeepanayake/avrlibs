/*
 * IncFile1.h
 *
 * Created: 24-Jul-24 11:11:28 AM
 * Author: Ranul Deepanayake
 * I2C bus scanner library for AVR microcontrollers.
 * Scans addresses from 0-127 using the write mode.
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

//Scans the I2C address range in the write mode.
void i2cScan();
//Supporting function for decimal to hexadecimal conversion.
void i2cScanDebugIntToHex(uint16_t input, char *temp_1);

/*
Example implementation

uartSet(UART_BAUD_RATE(9600), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);
i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
i2cScan();

*/

#endif /* INCFILE1_H_ */