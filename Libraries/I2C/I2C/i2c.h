/*
 * i2c.h
 *
 * Created: 01-Nov-18 4:52:34 PM
 * Author: Ranul Deepanayake
 * A set of basic functions to help understand I2C communication and ready to use I2C functions.
 */ 


#ifndef I2C_H_
#define I2C_H_

//Includes.
#include <avr/io.h>	//Pin definitions.
#include <avr/interrupt.h> //For the I2C interrupt vector.
#include <stdbool.h>

//CPU clock.
#ifndef F_CPU
#define F_CPU 16000000UL	
#endif

//I2C clock set to 100KHz (standard mode).
#ifndef SCL_CLOCK
#define SCL_CLOCK 100000UL	
#endif

//Pre-scaler for the I2C clock.
#ifndef I2C_PRESCALER
#define I2C_PRESCALER 1	
#endif

//Address used in slave mode.
#ifndef I2C_SLAVE_ADDRESS
#define I2C_SLAVE_ADDRESS 0x05
#endif

#define TWGCI 0x01
#define I2C_BAUD_RATE ((F_CPU/SCL_CLOCK)-16)/(2* I2C_PRESCALER)

//Slave status codes.
#define I2C_SLAVE_ADRRESS_POLLED 0x60
#define I2C_SLAVE_DATA_RECEIVED 0x80

enum i2_slave_write_read_mode{I2C_WRITE, I2C_READ};
enum i2c_master_transmitter_status_codes{SUCCESS, START_FAILED, REPEAT_START_FAILED, SLAVE_ADDRESS_UNACKNOWLEDGED, SLAVE_DATA_UNACKNOWLEDGED, MASTER_DATA_UNACKNOWLEDGED};

/*
Functions to initialize the the I2C bus.
*/
//For master mode initialization.
void i2cSetMaster(uint8_t prescaler, uint8_t baud_rate); 
//For slave mode initialization.
void i2cSetSlave(uint8_t prescaler, uint8_t baud_rate, uint8_t slave_address); 

/*
Functions which are very low level.
*/ 
void start();
uint8_t getStatus();
void write(uint8_t data);
uint8_t readAck();
uint8_t readNack();
void stop();

/*
Functions for single byte operations in the master mode.
*/
uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write);
uint8_t i2cWrite(uint8_t data);
uint8_t i2cReadByte();
uint8_t i2cReadLastByte();
void i2cStop();

/*
Functions for multi byte operations in the master mode.
*/
uint8_t masterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data);
uint8_t masterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data);

/*
Functions for receiving data in the slave mode.
*/
uint8_t i2cSlaveReadAck();
bool i2cSlaveReceivedNewData();

#endif /* I2C_H_ */