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
#include <avr/interrupt.h> //For the I2C interrupt vector.
#include <stdbool.h>

//CPU clock.
#ifndef F_CPU
#define F_CPU 16000000UL	
#endif

//I2C clock default set to 100KHz (standard mode).
#ifndef SCL_CLOCK
#define SCL_CLOCK 100000UL
//#define SCL_CLOCK 400000UL //400KHz
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

//Master status codes.

//Slave status codes.
#define I2C_SLAVE_ADRRESS_WRITE_POLLED 0x60
#define I2C_SLAVE_DATA_WRITE_RECEIVED 0x80
#define I2C_SLAVE_ADRRESS_READ_POLLED 0xA8
#define I2C_SLAVE_DATA_READ_RECEIVED 0xB8
#define I2C_SLAVE_DATA_READ_END 0x0C

enum i2_slave_write_read_mode{I2C_WRITE, I2C_READ};
enum i2c_master_transmitter_status_codes{SUCCESS, START_FAILED, REPEAT_START_FAILED, SLAVE_ADDRESS_UNACKNOWLEDGED, SLAVE_DATA_UNACKNOWLEDGED, MASTER_DATA_UNACKNOWLEDGED, COMMUNICATION_ERROR};

#ifdef __cplusplus

extern "C" void  TWI_vect(void) __attribute__ ((signal));

class I2C
{
	public:
		//Slave data receive buffer.
		uint8_t slave_write_data_received_buffer = 0;
		//Slave data received flag.
		bool slave_received_new_write_data = false;

		//Slave data send buffer.
		uint8_t slave_data_to_be_sent_buffer = 0;
		//Slave data sent flag.
		bool slave_sent_new_read_data = false;


	/*
	Class constructor.
	*/	
	public: 
		I2C();
		
		/*
		Functions to initialize the the I2C bus.
		*/
		void i2cSetMaster(uint8_t prescaler, uint8_t baud_rate);
		void i2cSetSlave(uint8_t prescaler, uint8_t baud_rate, uint8_t slave_address);
		
		/*
		Master send and receive functions.
		*/
		uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write);
		uint8_t i2cWrite(uint8_t data);
		uint8_t i2cReadByte();
		uint8_t i2cReadLastByte();
		void i2cStop();
		/*
		Slave send and receive functions.
		*/
		uint8_t i2cSlaveReadAck();
		bool i2cSlaveReceivedNewWriteData();
		void i2cSlaveSendData(uint8_t data);
		bool i2cSlaveSentNewReadData();
		void i2cSlaveSetMoreDataToSend();

		void i2cStart();
		uint8_t i2cGetStatus();
		void i2cWriteByte(uint8_t data);

		/*
		Functions for multi byte operations in the master mode.
		*/
		uint8_t i2cMasterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data);
		uint8_t i2cMasterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data);
		
		friend void TWI_vect(void);
};

#else

typedef
struct I2C
I2C;

#endif

/*
For C accessibility.
*/
#ifdef __cplusplus
extern "C" {
	#endif
	
	/*
		Functions to initialize the the I2C bus.
		*/
		void i2cSetMaster(uint8_t prescaler, uint8_t baud_rate);
		void i2cSetSlave(uint8_t prescaler, uint8_t baud_rate, uint8_t slave_address);
		
		/*
		Master send and receive functions.
		*/
		uint8_t i2cDelayedStart(uint8_t slave_address, uint8_t read_write);
		uint8_t i2cWrite(uint8_t data);
		uint8_t i2cReadByte();
		uint8_t i2cReadLastByte();
		void i2cStop();
		/*
		Slave send and receive functions.
		*/
		uint8_t i2cSlaveReadAck();
		bool i2cSlaveReceivedNewWriteData();
		void i2cSlaveSendData(uint8_t data);
		bool i2cSlaveSentNewReadData();
		void i2cSlaveSetMoreDataToSend();

		void i2cStart();
		uint8_t i2cGetStatus();
		void i2cWriteByte(uint8_t data);

		/*
		Functions for multi byte operations in the master mode.
		*/
		uint8_t i2cMasterRecieveData(uint8_t slave_address, uint8_t register_address, uint8_t number_of_bytes, uint8_t *data);
		uint8_t i2cMasterWriteData(uint8_t slave_address, uint8_t number_of_bytes, uint8_t *register_address, uint8_t *data);
	
	// The static class definition.
	extern I2C I2C_OBJECT;
	
	#ifdef __cplusplus
} // end extern C
#endif

/*
Example implementation.

int main(void)
{
	i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
	
	while (1)
	{
		bmp280Simple();
		
	}
}

*/

#endif /* I2C_H_ */