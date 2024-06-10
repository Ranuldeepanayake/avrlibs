/*
 * i2c.c
 *
 * Created: 02-Nov-18 7:30:24 PM
 * Author: Ranul Deepanayake
 * A set of basic functions to help understand I2C communication and ready to use I2C functions.
 */ 

#include "i2c.h"

/*================================================================================================================================================*/
/*===============================================FUNCTIONS TO HELP YOU UNDERSTAND I2C BUS OPERATION===============================================*/
/*================================================================================================================================================*/


void i2cSet(uint8_t prescaler, uint8_t baud_rate){
	TWSR= prescaler;	//Two wire status register.	////////////////////////////////////////Currently set to 100KHz.
	TWBR= baud_rate; //((F_CPU/SCL_CLOCK)-16)/2;	//Set I2C bit rate for SCL generation in master modes.
	TWCR= (1<< TWEN); //Enable the I2C interface.
	
}

void start(){
	//Send start signal on the I2C bus.
	TWCR= ((1<< TWINT) | (1<< TWSTA) | (1<< TWEN)); //Clear TWINT to execute start signal.
	while(!(TWCR & (1<< TWINT)));	//Wait for TWINT to become zero (wait for pending operations to finish).
}

void stop(){
	//Send stop signal on the I2C bus.
	//TWCR&= ~(1<< TWSTA);	//Disable the start signal.
	TWCR= ((1<< TWINT) | (1<< TWSTO) | (1<< TWEN));	//Set the stop bit.
	while(TWCR & (1<< TWSTO));	//Wait for TWSTO to be cleared automatically.
}

void write(uint8_t data){
	//Write data to the I2C bus (device addresses, register addresses and data).
	TWDR= data;
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Clear TWINT to shift data out the I2C bus.
	while(!(TWCR & (1<< TWINT)));
}

uint8_t getStatus(){
	//Return status of an I2C operation. Get the value by masking the pre-scaler bits.
	return TWSR & 0xF8;
}

uint8_t readAck(){
	TWCR= ((1<< TWINT) | (1<< TWEN) | (1<< TWEA));	//Enable ACK. Sends ACK to sender after receiving data.
	while(!(TWCR & (1<< TWINT)));
	return TWDR;
}

uint8_t readNack(){
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Disable ACK. Sends NACK to sender after receiving data to signal the sender to stop sending data.
	while(!(TWCR & (1<< TWINT)));
	return TWDR;
}


/*================================================================================================================================================*/
/*===============================================READY TO USE FUNCTIONS FOR SINGLE BYTE READS AND WRITES==========================================*/
/*================================================================================================================================================*/

uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write){
	//Start signal on the I2C bus.
	TWCR= ((1<< TWINT) | (1<< TWSTA) | (1<< TWEN)); //Clear TWINT to execute start signal. Any operation on I2C hardware will only execute if TWINT is written to 1. Clearing TWINT is done by writing a 1 while setting TWINT is done by writing a 0. I know...but that's how it is.
	while(!(TWCR & (1<< TWINT)));	//Wait for TWINT to become zero (wait for pending operations to finish).
	
	if(read_write== I2C_WRITE && getStatus()!= 0x08){
		stop();	//End transaction and release bus if start fails.
		return START_FAILED;	//Get start signal status.
	}
	
	if(read_write== I2C_READ && getStatus()!= 0x10){
		stop();	//End transaction and release bus if repeat start fails.
		return REPEAT_START_FAILED;
	}
	
	write((slave_address<< 1) | read_write);	//Send slave address+ write option.
	
	if(read_write== I2C_WRITE && getStatus()!= 0x18){
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	if(read_write== I2C_READ && getStatus()!= 0x40){
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	return SUCCESS;
}

uint8_t i2cWrite(uint8_t data){
	//Write data to the I2C bus (device addresses, register addresses and data).
	TWDR= data;
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Clear TWINT to shift data out the I2C bus.
	while(!(TWCR & (1<< TWINT)));
	
	if(getStatus()!= 0x28){	//Check if slave has acknowledged the sent data.
		stop();	//End transaction and release bus if ack not received.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	return SUCCESS;
}

uint8_t i2cReadByte(){
	TWCR= ((1<< TWINT) | (1<< TWEN) | (1<< TWEA));	//Enable ACK. Sends ACK to sender after receiving data.
	while(!(TWCR & (1<< TWINT)));
	
	if(getStatus()!= 0x50){	//Check if slave has acknowledged the sent data.
		stop();	//End transaction and release bus if ack not received.
		return MASTER_DATA_UNACKNOWLEDGED;
	}
	return TWDR;	//If ack is sent, return data.
}

uint8_t i2cReadLastByte(){
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Disable ACK. Creates a NACK condition which signals the slave to stop sending data.
	while(!(TWCR & (1<< TWINT)));
	return TWDR;	//If ack is sent, return data.
}

void i2cStop(){
	//Sends the stop signal on the I2C bus.
	TWCR= ((1<< TWINT) | (1<< TWSTO) | (1<< TWEN));	//Set the stop bit.
	while(TWCR & (1<< TWSTO));	//Wait for TWSTO to be cleared automatically.
}


/*================================================================================================================================================*/
/*================================================READY TO USE FUNCTIONS FOR MASS READS AND WRITES================================================*/
/*================================================================================================================================================*/

uint8_t masterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data){
	//Reads multiple bytes from the slave.
	//Send command to slave in write mode.
	i2cDelayedStart(slave_address, I2C_WRITE);
	i2cWrite(register_address);
	//Send command to slave in read mode and prepare to receive data from slave.
	i2cDelayedStart(slave_address, I2C_READ);
	for(int i= 0; i< number_of_bytes; i++){
		if(i+ 1== number_of_bytes){
			*(data+ i)= i2cReadLastByte();
			}else{
			*(data+ i)= i2cReadByte();
		}
	}
	stop();
	return SUCCESS;
}

uint8_t masterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data){
	//Write multiple bytes to the slave.
	//Send command to slave in write mode.
	i2cDelayedStart(slave_address, I2C_WRITE);
	for(int i= 0; i< number_of_bytes; i++){
		i2cWrite(*(register_address+ i));	//Send register address.
		i2cWrite(*(data+ i));	//Send register data.	
	}
	stop();
	return SUCCESS;
}


