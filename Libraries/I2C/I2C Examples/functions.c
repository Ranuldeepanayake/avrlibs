/*
 * functions.c
 *
 * Created: 06-Nov-18 9:21:36 AM
 * Author: Ranul Deepanayake
 */ 
#include "examples.h"

char data_string[10];
char status_string[10];

uint8_t bmp280Simple(){
	/* In this function, we are attempting to connect to the BOSCH BMP280 via I2C and write and read one byte of data.
	   Section A is for writing a byte and section B is for reading a byte.	
	   The steps of writing are,
	   A1. Send the start signal on the I2C bus.
	   A2. Check whether the start signal was sent.
	   A3. Send the intended slave's address on the I2C bus *which has to include the read/write mode as well which is write in this case*.
	   A4. Get the acknowledgment for the sent slave address from the slave. 
	   A5. Send the register address of the slave to which data is to be written to.
	   A6. Get the acknowledgment for the sent register address from the slave. 	
	   A7. Send the data byte to the slave.
	   A8. Get the acknowledgment for the sent data byte from the slave. 
	   A9. Send the stop signal on the I2C bus.
	   
	   B1. Send the start signal on the I2C bus.
	   B2. Check whether the start signal was sent.
	   B3. Send the intended slave's address on the I2C bus *which has to include the read/write mode as well which is write in this case*.
	   B4. Get the acknowledgment for the sent slave address from the slave.
	   B5. Send the register address of the slave from which data is to be read.
	   B6. Get the acknowledgment for the sent register address from the slave.
	   B7. Send the stop signal on the I2C bus.
	   B8. Send the start signal on the I2C bus.
	   B9. Check whether the start signal was sent.
	   B10. Send the intended slave's address on the I2C bus *which has to include the read/write mode as well which is read in this case*.
	   B11. Get the acknowledgment for the sent slave address from the slave.
	   B12. Read one byte of data from the slave and send a NACK (no acknowledgment) to the slave.
	   B13. Send the stop signal on the I2C bus.
	*/
	uint8_t register_value= 0;
	
	//Send one byte of data to the sensor to set the measurement configuration.
	
	start();	//A1.
	if(getStatus()!= 0x08){	//A2.
		stop();	//End transaction and release bus if start fails.
		return START_FAILED;	
	}
	
	write((0x76<< 1) | 0);	//A3.
	if(getStatus()!= 0x18){	//A4.
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	write(0xF4);	//A5.
	if(getStatus()!= 0x28){	//A6.
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	
	write(0x2F);	//A7.
	if(getStatus()!= 0x28){	//A8.
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	
	stop(); //A9.
	
	//Read one byte of data from the sensor. For that a write operation has to be done to select which register to be read from.
	
	start();
	if(getStatus()!= 0x08){
		stop();	//End transaction and release bus if start fails.
		return START_FAILED;	//Get start signal status.
	}
	
	write((0x76<< 1) | 0);
	if(getStatus()!= 0x18){
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	write(0xFA);
	if(getStatus()!= 0x28){	//Check if slave has acknowledged the sent data.
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	
	stop();
	
	//Do the actual reading.
	
	start();
	if(getStatus()!= 0x08){
		stop();	//End transaction and release bus if start fails.
		return START_FAILED;	//Get start signal status.
	}
	
	write((0x76<< 1) | 1);
	if(getStatus()!= 0x40){
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	register_value= readNack();
	
	stop();
	
	
	sprintf(data_string, "%d", register_value);
	print("Register Value: "); print(data_string); println("\n");
	_delay_ms(500);
	return SUCCESS;
}

void bmp280RepeatStart(){
	//Set config and read value.
}

void eepromSimple(){
	//Write and read one byte.
}

void eepromMultiple(){
	//Write and read several bytes.
}
