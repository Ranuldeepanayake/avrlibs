/*
 * i2c.c
 *
 * Created: 02-Nov-18 7:30:24 PM
 * Author: Ranul Deepanayake
 */ 

#include "i2c.h"

/*
Set up the I2C peripheral. Use the macro 'I2C_BAUD_RATE(I2C_SCL_CLOCK)' to set the SCL speed. 
Default SCL speed is 100KHz.
*/
void i2c_set(uint8_t baud_rate){
	TWSR= I2C_PRESCALER; 
	TWBR= baud_rate;	//((F_CPU/SCL_CLOCK)-16)/2;	//Set I2C bit rate for SCL generation in master modes.
	TWCR= (1<< TWEN);	//Enable the I2C interface.	
}

/*
Send a delayed start (and start) condition on the I2C bus. 
Automatically shifts the address bits to accommodate the R/W mode.
*/
uint8_t i2c_delayed_start(uint8_t slave_address, uint8_t read_write){
	TWCR= ((1<< TWINT) | (1<< TWSTA) | (1<< TWEN)); //Clear TWINT to execute start signal. Any operation on I2C hardware will only execute if TWINT is written to 1. Clearing TWINT is done by writing a 1 while setting TWINT is done by writing a 0. I know...but that's how it is.
	while(!(TWCR & (1<< TWINT)));	//Wait for TWINT to become zero (wait for pending operations to finish).
	
	if(read_write== I2C_WRITE && i2c_status()!= I2C_START_FAILED){
		i2c_stop();		//End transaction and release bus if start fails.
		return I2C_START_FAILED;	//Get start signal status.
	}
	
	if(read_write== I2C_READ && i2c_status()!= I2C_REPEAT_START_FAILED){
		i2c_stop();		//End transaction and release bus if repeat start fails.
		return I2C_REPEAT_START_FAILED;
	}
	
	TWDR= ((slave_address<< 1) | read_write);	//Send slave address+ write option.
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Clear TWINT to shift data out the I2C bus.
	while(!(TWCR & (1<< TWINT)));
	
	if(read_write== I2C_WRITE && i2c_status()!= I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_WRITE_MODE){
		i2c_stop();		//End transaction and release bus if ack not received.
		return I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_WRITE_MODE;
	}
	
	if(read_write== I2C_READ && i2c_status()!= I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_READ_MODE){
		i2c_stop();		//End transaction and release bus if ack not received.
		return I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_READ_MODE;
	}
	return I2C_SUCCESS;
}

/*
Send one byte of data over the I2C bus.
*/
uint8_t i2c_write(uint8_t data){
	//
	TWDR= data;
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Clear TWINT to shift data out the I2C bus.
	while(!(TWCR & (1<< TWINT)));
	
	if(i2c_status()!= I2C_SLAVE_DATA_UNACKNOWLEDGED){	//Check if slave has acknowledged the sent data.
		i2c_stop();		//End transaction and release bus if ack not received.
		return I2C_SLAVE_DATA_UNACKNOWLEDGED;
	}
	return I2C_SUCCESS;
}

/*
Read one byte of data over the I2C bus and send an acknowledgment to the slave.
*/
uint8_t i2c_read_ack(){
	TWCR= ((1<< TWINT) | (1<< TWEN) | (1<< TWEA));	//Enable ACK. Sends ACK to sender after receiving data.
	while(!(TWCR & (1<< TWINT)));
	
	if(i2c_status()!= I2C_MASTER_DATA_UNACKNOWLEDGED){	//Check if slave has acknowledged the sent data.
		i2c_stop();	//End transaction and release bus if ack not received.
		return I2C_MASTER_DATA_UNACKNOWLEDGED;
	}
	return TWDR;	//If ack is sent, return data.
}

/*
Read one byte of data over the I2C bus and without sending an acknowledgment to the slave.
*/
uint8_t i2c_read_nack(){
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Disable ACK. Creates a NACK condition which signals the slave to stop sending data.
	while(!(TWCR & (1<< TWINT)));
	return TWDR;	//Return data.
}

/*
Send a stop condition over the I2C bus.
*/
void i2c_stop(){
	//Sends the stop signal on the I2C bus.
	TWCR= ((1<< TWINT) | (1<< TWSTO) | (1<< TWEN));	//Set the stop bit.
	while(TWCR & (1<< TWSTO));	//Wait for TWSTO to be cleared automatically.
}

/*
Get the status of the last operation carried out by the I2C peripheral.
*/
uint8_t i2c_status(){
	//Return status of an I2C operation. Get the value by masking the pre-scaler bits.
	return TWSR & I2C_STATUS_BITS;
}


