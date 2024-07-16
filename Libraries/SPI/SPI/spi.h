/*
 * SPI.h
 *
 * Created: 01-Jul-24 12:46:14 PM
 * Author : Ranul Deepanayake
 * A SPI library for the ATmega 328P. Uses the SPI hardware peripheral.
 */ 

#ifndef SPI_H_
#define SPI_H_

//Define the CPU frequency.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//Polarity and phase modes.
#define SPI_POLARITY_0_PHASE_0 0x00
#define SPI_POLARITY_0_PHASE_1 0x04
#define SPI_POLARITY_1_PHASE_0 0x08
#define SPI_POLARITY_1_PHASE_1 0x0C

//SPI clock rate selection. SPI clock rate = CPU speed / SPI_CLOCK_*. The CPU speed must be at least 2 x the desired SPI clock rate.
#define SPI_CLOCK_4	  0x00
#define SPI_CLOCK_16  0x01	
#define SPI_CLOCK_64  0x02
#define SPI_CLOCK_128 0x03

//LSB/MSB selection.
#define SPI_DATA_ORDER_MSB 0
#define SPI_DATA_ORDER_LSB 1

//CS default pin register.
#define SPI_CS_DDR_REGISTER_DEFAULT 0x04
#define SPI_CS_PORT_REGISTER_DEFAULT 0x05

/*
Functions.
*/
//Initialize the SPI peripheral.
void spiSetMaster(uint8_t spi_clock_rate, uint8_t spi_polarity_phase, uint8_t spi_data_order);
//Initialize the SPI peripheral with common parameters.
void spiSetMasterDefault();
//Set the SC pin.
void spiSetCSpin(uint16_t ddr_register, uint8_t pin);
//Unset the CS pin.
void spiUnsetCSpin(uint16_t ddr_register, uint8_t pin);
//Drive the CS pin high.
void spiCSHigh(uint16_t output_register, uint8_t pin);
//Drive the CS pin low.
void spiCSLow(uint16_t output_register, uint8_t pin);
//Transmit one byte on the SPI bus.
char spiTransmitByte(char byte);
//A test function which can be used with logic analyzer tests
void spiTest();

/*

Example implementation.

//Initialize the SPI peripheral.
spiSetMaster(SPI_CLOCK_4, SPI_POLARITY_0_PHASE_0, SPI_DATA_ORDER_LSB, DDRB, PORTB, PORTB2);
//Set the CS pin.
spiSetCSpin(DDRB, DDB2);
//Set the CS pin high.
spiCSHigh(PORTB, PORTB2);
//Send a command + value.
spiTransmitByte(0xA1);
spiTransmitByte(0x33);
//Receive the corresponding data by transmitting empty bytes.
char lb = spiTransmitByte(0x00);
char hb = spiTransmitByte(0x00);
//Set the CS pin low.
spiCSLow(PORTB, PORTB2);

*/

#endif /* SPI_H_ */