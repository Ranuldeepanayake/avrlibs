/*
 * i2c.h
 *
 * Created: 01-Nov-18 4:52:34 PM
 * Author: Ranul Deepanayake
 * A set of basic functions to help understand I2C communication and ready to use I2C functions.
 */ 


#ifndef I2C_H_
#define I2C_H_

#include <avr/io.h>	//Pin definitions.

#ifndef F_CPU
#define F_CPU 16000000UL	//CPU clock.
#endif

#ifndef SCL_CLOCK
#define SCL_CLOCK 100000UL	//I2C clock set to 100KHz (standard mode).
#endif

#ifndef I2C_PRESCALER
#define I2C_PRESCALER 1	//Pre-scaler for the I2C clock.
#endif

#define I2C_BAUD_RATE ((F_CPU/SCL_CLOCK)-16)/(2* I2C_PRESCALER)

enum i2_slave_write_read_mode{I2C_WRITE, I2C_READ};
enum i2c_master_transmitter_status_codes{SUCCESS, START_FAILED, REPEAT_START_FAILED, SLAVE_ADDRESS_UNACKNOWLEDGED, SLAVE_DATA_UNACKNOWLEDGED, MASTER_DATA_UNACKNOWLEDGED};

void i2cSet(uint8_t prescaler, uint8_t baud_rate);

void start();
uint8_t getStatus();
void write(uint8_t data);
uint8_t readAck();
uint8_t readNack();
void stop();

uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write);
uint8_t i2cWrite(uint8_t data);
uint8_t i2cReadByte();
uint8_t i2cReadLastByte();
void i2cStop();

uint8_t masterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data);
uint8_t masterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data);

#endif /* I2C_H_ */