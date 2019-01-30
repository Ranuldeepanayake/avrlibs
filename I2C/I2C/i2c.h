/*
 * i2c.h
 *
 * Created: 01-Nov-18 4:52:34 PM
 * Author: Ranul Deepanayake
 * Hardware I2C library for the ATmega328P. Supports single master transmit/receive only.
 */ 

#ifndef I2C_H_
#define I2C_H_

//Includes.
#include <avr/io.h>	//Pin definitions.

//Attributes.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#ifndef I2C_SCL_CLOCK
#define I2C_SCL_CLOCK 100000UL	//I2C clock default set to 100KHz (standard mode).
#endif
#ifndef I2C_PRESCALER
#define I2C_PRESCALER 1	
#endif
#define I2C_BAUD_RATE(SCL_CLOCK_SPEED) ((F_CPU/SCL_CLOCK_SPEED)-16)/(2* I2C_PRESCALER)	
#define I2C_WRITE 0
#define I2C_READ 1
#define I2C_STATUS_BITS 0xF8

//Error and status codes.	
#define	I2C_SUCCESS 0
#define	I2C_START_FAILED 0x08
#define	I2C_REPEAT_START_FAILED 0x10
#define	I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_WRITE_MODE 0x18
#define	I2C_SLAVE_ADDRESS_UNACKNOWLEDGED_READ_MODE 0x40
#define	I2C_SLAVE_DATA_UNACKNOWLEDGED 0x28
#define	I2C_MASTER_DATA_UNACKNOWLEDGED 0x50

//Functions.
//Set up the I2C peripheral. Use the macros 'I2C_BAUD_RATE(I2C_SCL_CLOCK)' to set the SCL speed.
void i2c_set(uint8_t baud_rate);
//Send delayed start condition with slave address and R/W mode.
uint8_t i2c_delayed_start(uint8_t slave_address, uint8_t read_write);
//Send byte on the I2C line.
uint8_t i2c_write(uint8_t data);
//Read byte with acknowledgment.
uint8_t i2c_read_ack();
//Read byte without acknowledgment.
uint8_t i2c_read_nack();
//Send stop condition.
void i2c_stop();
//Get status from the I2C peripheral.
uint8_t i2c_status();

/*
Example implementation. Read three bytes from an I2C slave.

#include "i2c.h"

void main(){
	i2c_set(I2C_BAUD_RATE(I2C_SCL_CLOCK));
	
	while(1){
		i2c_delayed_start(SLAVE_ADDRESS, I2C_WRITE);
		i2c_write(COMMAND_BYTE);
		i2c_delayed_start(SLAVE_ADDRESS, I2C_READ);
		uint8_t b_1= i2c_read_ack();
		uint8_t b_2= i2c_read_ack();
		uint8_t b_3= i2c_read_nack();
		i2c_stop();
	}
}

*/

#endif /* I2C_H_ */