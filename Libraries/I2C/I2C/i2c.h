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
 */ 


#ifndef I2C_H_
#define I2C_H_

//Includes.
#include <avr/io.h>	//Pin definitions.
#include <stdbool.h>

//CPU clock.
#ifndef F_CPU
#define F_CPU 16000000UL	
#endif

//I2C clock default set to 100KHz (standard mode).
#ifndef SCL_CLOCK
#define SCL_CLOCK 100000UL //100KHz
//#define SCL_CLOCK 400000UL //400KHz
#endif

//Prescaler for the I2C clock.
#ifndef I2C_PRESCALER
#define I2C_PRESCALER 1	
#endif

//Default Address used in slave mode.
#ifndef I2C_SLAVE_ADDRESS
#define I2C_SLAVE_ADDRESS 0x05
#endif

//Bus hangup detection timer clock cycles (number of I2C clock cycles).
#ifndef BUS_HANGUP_TIMEOUT_CYCLES
#define BUS_HANGUP_TIMEOUT_CYCLES 60
#endif

#define BUS_HANGUP_PROTECTION_ENABLED

//Bus hangup detection timer.
#ifndef BUS_HANGUP_TIMEOUT
#define BUS_HANGUP_TIMEOUT 640 //Directly in microseconds. Value obtained after noticing that one I2C clock with the BMP280 spanned 40 microseconds (40 x 16 cycles).
//#define BUS_HANGUP_TIMEOUT (((1 / (SCL_CLOCK/1000)) * 1000) * BUS_HANGUP_TIMEOUT_CYCLES) //This macro did not perform proper math??
#endif

#define TWGCI 0x01

//Baud rate calculation.
#define I2C_BAUD_RATE ((F_CPU/SCL_CLOCK)-16)/(2* I2C_PRESCALER)

//Master status codes.

//Slave status codes.
#define I2C_SLAVE_ADRRESS_WRITE_POLLED 0x60
#define I2C_SLAVE_DATA_WRITE_RECEIVED 0x80
#define I2C_SLAVE_ADRRESS_READ_POLLED 0xA8
#define I2C_SLAVE_DATA_READ_RECEIVED 0xB8
#define I2C_SLAVE_DATA_READ_END 0x0C

enum i2_slave_write_read_mode{I2C_WRITE, I2C_READ};
enum i2c_master_transmitter_status_codes{SUCCESS, START_FAILED, REPEAT_START_FAILED, SLAVE_ADDRESS_UNACKNOWLEDGED, SLAVE_DATA_UNACKNOWLEDGED, MASTER_DATA_UNACKNOWLEDGED, COMMUNICATION_ERROR};

/*
Functions to initialize the the I2C bus.
*/
//For master mode initialization.
void i2cSetMaster(uint8_t prescaler, uint8_t baud_rate); 
//For slave mode initialization.
void i2cSetSlave(uint8_t prescaler, uint8_t baud_rate, uint8_t slave_address); 

/*
Master send and receive functions. 
*/
//Start communication with a slave.
uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write);
//Send a command or data byte to a slave.
uint8_t i2cWrite(uint8_t data);
//Read a data byte from a slave and send an ACK.
uint8_t i2cReadByte();
//Read a data byte from a slave and send a NACK.
uint8_t i2cReadLastByte();
//Generate a stop condition.
void i2cStop();

/*
Slave send and receive functions. 
*/
//Returns write a command data byte in the receive buffer received from the master.
uint8_t i2cSlaveReadAck();
//Check whether the slave received a write command from the master.
bool i2cSlaveReceivedNewWriteData();
//Places a data byte in the transmit buffer to be sent to the master.
void i2cSlaveSendData(uint8_t data);
//Check whether the slave received a read command from the master.
bool i2cSlaveSentNewReadData();
//Resets the slave sent data flag to accommodate additional data bytes to be sent to the master.
void i2cSlaveSetMoreDataToSend();

/*
Internal functions.
*/
//Generate a start condition.
void i2cStart();
//Get the status code from the I2C status register.
uint8_t i2cGetStatus();
//Send a byte on the bus.
void i2cWriteByte(uint8_t data);

/*
Functions for multi byte operations in the master mode.
*/
//Write a stream of data to a slave.
uint8_t i2cMasterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data);
//Read a stream of data from a slave.
uint8_t i2cMasterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data);

/*
Example implementation.

int main(void)
{
	i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
	
	while (1)
	{
		i2cDelayedStart(BMP280_ADDRESS, I2C_WRITE);
		i2cWrite(BMP280_MEASUREMENT_CONTROL_REGISTER);
		i2cDelayedStart(BMP280_ADDRESS, I2C_READ);
		temp_msb= i2cReadByte();
		temp_lsb= i2cReadByte();
		temp_xlsb= i2cReadLastByte();
		i2cStop();
	}
}

*/

#endif /* I2C_H_ */