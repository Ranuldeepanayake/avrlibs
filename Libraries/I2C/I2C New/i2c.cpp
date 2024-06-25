/*
 * i2c.c
 *
 * Created: 02-Nov-18 7:30:24 PM
 * Author: Ranul Deepanayake
 * I2C library for the ATmega 328P.
 * Uses blocking code for master send and receive modes.
 * Uses interrupt based code for slave send and receive modes.
 * Master modes do not use a buffer.
 * The slave modes uses a single byte buffer.
 
 https://waterproofman.wordpress.com/2007/02/07/avr-interrupts-in-c/
 */ 

#include "i2c.h"

I2C I2C_OBJECT;

/*
I2C receive ISR. This ISR is entered when the I2C bus receives a byte from a master or slave. This ISR is currently used only for the two slave modes.
***Still under development. 
*/
ISR (TWI_vect){
	//When addressed with an address poll with a write bit, there will be no action to take.
	if(TWSR & I2C_SLAVE_ADRRESS_WRITE_POLLED){ TWCR |= (1 << TWINT);}
	
	//When data is sent by the master with a write bit. This is usually a command.
	if(TWSR & I2C_SLAVE_DATA_WRITE_RECEIVED){ I2C_OBJECT.slave_write_data_received_buffer = TWDR; I2C_OBJECT.slave_received_new_write_data = true; TWCR |= (1 << TWINT);}
	
	//When addressed with an address poll with a read bit, the first data byte will be sent to the master and the master will send an ack. This is usually the first request for data from the slave..
	if(TWSR & I2C_SLAVE_ADRRESS_READ_POLLED){ TWDR = I2C_OBJECT.slave_data_to_be_sent_buffer; I2C_OBJECT.slave_sent_new_read_data = true; TWCR |= (1 << TWINT);}
	
	//When the master acknowledges a previously received byte and an ack is sent to the slave. The next data byte will be sent to the master and the master will send an ack.
	//This is usually a continuation of a request for data from the slave..
	if(TWSR & I2C_SLAVE_DATA_READ_RECEIVED){ TWDR = I2C_OBJECT.slave_data_to_be_sent_buffer; I2C_OBJECT.slave_sent_new_read_data = true; TWCR |= (1 << TWINT);}
	
	//When the master does not acknowledge a previously received byte and an nack is sent to the slave. This is usually the end of request for data from the slave.
	//The buffer is zeroed for reliability.
	if(TWSR & I2C_SLAVE_DATA_READ_END){ I2C_OBJECT.slave_data_to_be_sent_buffer = 0; I2C_OBJECT.slave_sent_new_read_data = false; TWCR |= (1 << TWINT);}
}

/*
Initialization functions for all modes.
*/

/*
Sets the I2C peripheral to operate in the master send and receive modes.
*/
void I2C :: i2cSetMaster(uint8_t prescaler, uint8_t baud_rate){
	TWSR= prescaler;	//Two wire status register.	////////////////////////////////////////Currently set to 100KHz.
	TWBR= baud_rate; //((F_CPU/SCL_CLOCK)-16)/2;	//Set I2C bit rate for SCL generation in master modes.
	TWCR= (1<< TWEN); //Enable the I2C interface.
	
}

/*
Sets the I2C peripheral to operate in the slave send and receive modes. Enables global interrupts since the I2C Rx ISR is used.
*/
void I2C :: i2cSetSlave(uint8_t prescaler, uint8_t baud_rate, uint8_t slave_address){
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
	TWAR = (slave_address << TWGCI);
	sei();
}

/*
Below are functions used in the master send and master receive modes.
*/

/*
Initiates an I2C delayed start. This generates a start signal, addresses a specified slave in read or write mode. 
A while loop is utilized, hence there is a risk of the bus hanging up during an erroneous condition.
Returns a result code.
*/
uint8_t I2C :: i2cDelayedStart(uint8_t slave_address, uint8_t read_write){
	//Start signal on the I2C bus.
	i2cStart();	
	
	if(read_write== I2C_WRITE && i2cGetStatus()!= 0x08){
		i2cStop();	//End transaction and release bus if start fails.
		return START_FAILED;	//Get start signal status.
	}
	
	if(read_write== I2C_READ && i2cGetStatus()!= 0x10){
		i2cStop();	//End transaction and release bus if repeat start fails.
		return REPEAT_START_FAILED;
	}
	
	i2cWriteByte((slave_address<< 1) | read_write);	//Send slave address+ write option.
	
	if(read_write== I2C_WRITE && i2cGetStatus()!= 0x18){
		i2cStop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	if(read_write== I2C_READ && i2cGetStatus()!= 0x40){
		i2cStop();	//End transaction and release bus if ack not received.
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	return SUCCESS;
}

/*
Sends one byte of data on the I2C bus. A data byte can be an address, write command, read command or data. 
A while loop is utilized, hence there is a risk of the bus hanging up during an erroneous condition.
Returns a result code. 
*/
uint8_t I2C :: i2cWrite(uint8_t data){
	i2cWriteByte(data);
	
	if(i2cGetStatus()!= 0x28){	//Check if slave has acknowledged the sent data.
		i2cStop();	//End transaction and release bus if ack not received.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	return SUCCESS;
}

/*
Read one byte to be received from the I2C slave and send an acknowledgment. 
A while loop is utilized, hence there is a risk of the bus hanging up during an erroneous condition.
Returns a result code.
*/
uint8_t I2C :: i2cReadByte(){
	TWCR= ((1<< TWINT) | (1<< TWEN) | (1<< TWEA));	//Enable ACK. Sends ACK to sender after receiving data.
	//TWCR= ((1<< TWEN) | (1<< TWEA));	//Enable ACK. Sends ACK to sender after receiving data. //***New
	while(!(TWCR & (1<< TWINT)));
	
	if(i2cGetStatus()!= 0x50){	//Check if the master has acknowledged the received data.
		i2cStop();	//End transaction and release bus if ack not received.
		return MASTER_DATA_UNACKNOWLEDGED;
	}
	return TWDR;	//If ack is sent, return data.
}

/*
Read the last byte to be received from the I2C slave and not send an acknowledgment.
A while loop is utilized, hence there is a risk of the bus hanging up during an erroneous condition.
Returns a result code.
*/
uint8_t I2C :: i2cReadLastByte(){
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Disable ACK. Creates a NACK condition which signals the slave to stop sending data.
	//TWCR= ((1 << TWEN) | (0 << TWEA));	//Disable ACK. Creates a NACK condition which signals the slave to stop sending data. //***New
	while(!(TWCR & (1<< TWINT)));
	
	if(i2cGetStatus()!= 0x58){	//Check if the master has not acknowledged the received data. This is normal for the last byte. //***New
		return COMMUNICATION_ERROR;
	}
	return TWDR;	//If ack is sent, return data.
}

/*
Generates a stop condition on the I2C bus.
*/
void I2C :: i2cStop(){
	//Send stop signal on the I2C bus.
	//TWCR&= ~(1<< TWSTA);	//Disable the start signal.
	TWCR= ((1<< TWINT) | (1<< TWSTO) | (1<< TWEN));	//Set the stop bit.
	while(TWCR & (1<< TWSTO));	//Wait for TWSTO to be cleared automatically.
}

/*
Below are functions used in the slave send and slave receive modes.
*/

/*
Returns write a command data byte in the receive buffer received from the master.
*/
uint8_t I2C :: i2cSlaveReadAck(){
	slave_received_new_write_data = false;
	return slave_write_data_received_buffer;
}

/*
Check whether the slave received a write command from the master.
Returns true if there is an outstanding command and false if there isn't. 
*/
bool I2C :: i2cSlaveReceivedNewWriteData(){
	return slave_received_new_write_data;
}

/*
Places a data byte in the transmit buffer to be sent to the master.
*/
void I2C :: i2cSlaveSendData(uint8_t data){
	slave_data_to_be_sent_buffer = data;
}

/*
Check whether the slave received a read command from the master.
Returns true if there is an outstanding command and false if there isn't.
*/
bool I2C :: i2cSlaveSentNewReadData(){
	return slave_received_new_write_data;
}

/*
Resets the slave sent data flag to accommodate additional data bytes to be sent to the master.
This does not need to be called for the last data byte to be sent. 
*/
void I2C :: i2cSlaveSetMoreDataToSend(){
	slave_sent_new_read_data = false;
}

/*
Below are internal functions which do not need to be called directly.
*/

/*
Generates a start condition on the I2C bus.
*/
void I2C :: i2cStart(){
	//Send start signal on the I2C bus.
	TWCR= ((1<< TWINT) | (1<< TWSTA) | (1<< TWEN)); //Clear TWINT to execute start signal.
	while(!(TWCR & (1<< TWINT)));	//Wait for TWINT to become zero (wait for pending operations to finish).
}

/*
Sends one byte of data on the I2C bus. A data byte can be an address, write command, read command or data.
A while loop is utilized, hence there is a risk of the bus hanging up during an erroneous condition.
*/
void I2C :: i2cWriteByte(uint8_t data){
	//Write data to the I2C bus (device addresses, register addresses and data).
	TWDR= data;
	TWCR= ((1<< TWINT) | (1<< TWEN));	//Clear TWINT to shift data out the I2C bus.
	while(!(TWCR & (1<< TWINT)));
}

/*
Returns the status code of the last operation stored in the I2C status register. 
*/
uint8_t I2C :: i2cGetStatus(){
	//Return status of an I2C operation. Get the value by masking the pre-scaler bits.
	return TWSR & 0xF8;
}


/*
Below functions can be used for mass data reads and writes such as EEPROM read and write operations in the master modes.
*/

/*
Sends a command to an I2C slave requesting to read a string of data.
*/
uint8_t I2C :: i2cMasterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data){
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
	i2cStop();
	return SUCCESS;
}

/*
Sends a command to an I2C slave to write a string of data.
*/
uint8_t I2C :: i2cMasterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data){
	//Send command to slave in write mode.
	i2cDelayedStart(slave_address, I2C_WRITE);
	for(int i= 0; i< number_of_bytes; i++){
		i2cWrite(*(register_address+ i));	//Send register address.
		i2cWrite(*(data+ i));	//Send register data.	
	}
	i2cStop();
	return SUCCESS;
}

