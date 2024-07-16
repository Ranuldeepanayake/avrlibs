/*
 * SPI.c
 *
 * Created: 01-Jul-24 12:46:14 PM
 * Author : Ranul Deepanayake
 * A SPI library for the ATmega 328P. Uses the SPI hardware peripheral.
 */ 

#include <avr/io.h>
#include "SPI.h"

/*
Sets up the SPI peripheral with user provided parameters and sets the CS/MOSI/CLK pins as outputs.
*/
void spiSetMaster(uint8_t spi_clock_rate, uint8_t spi_polarity_phase, uint8_t spi_data_order){
	//Enable SPI, set the clock divisor, polarity + phase, master mode and the data order.
	SPCR = (1 << SPE) | spi_clock_rate | (1 << MSTR) | spi_polarity_phase | (spi_data_order << DORD);
	
	//Set the CS, MOSI and SCK pins as outputs. Here, the the default CS pin has also been set as an output since it accidentally being an input would change the master
	//operation behavior of SPI .
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
}

/*
Sets up the SPI peripheral with commonly used parameters.
Enables SPI, sets the clock divisor to 4, selects the master mode, sets 0,0 as the phase and polarity, sets MSB as the data order and sets the 
CS/MOSI/CLK pins as outputs.
*/
void spiSetMasterDefault(){
	//Enable SPI, set the clock divisor, polarity + phase, master mode and the data order.
	SPCR = (1 << SPE) | SPI_CLOCK_4 | (1 << MSTR) | SPI_POLARITY_0_PHASE_0 | (SPI_DATA_ORDER_MSB << DORD);
	
	//Set the CS, MOSI and SCK pins as outputs.
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
}

/*
Sets a pin as an output to be used as the CS pin.
*/
void spiSetCSpin(uint16_t ddr_register, uint8_t pin){
	_SFR_IO8(ddr_register) |= (1 << pin);
}

/*
Clears a pin set as an output to be used as the CS pin.
*/
void spiUnsetCSpin(uint16_t ddr_register, uint8_t pin){
	_SFR_IO8(ddr_register) &= ~(1 << pin);
}

/*
Sets a desired CS pin high to start communication with a slave.
*/
void spiCSHigh(uint16_t output_register, uint8_t pin){
	_SFR_IO8(output_register) |= (1 << pin);
}

/*
Sets a desired CS pin low to stop communication with a slave.
*/
void spiCSLow(uint16_t output_register, uint8_t pin){
	_SFR_IO8(output_register) &= ~(1 << pin);
}

/*
Transmits and receives one byte of data simultaneously on the SPI bus. The received byte will be in response to a previous byte sent by the master.
Therefore, to receive data for a command sent to a slave, the master must sent extra empty bytes corresponding to the number of bytes to be expected from the slave.
*/
char spiTransmitByte(char byte){
	char data = 0x00;
	
	//Load the data byte into the data register. Data will be shifted out immediately.
	SPDR = byte;
	
	//***Use a timer for bus hangup detection.
	//Wait for the transmission complete flag to be set.
	while(!(SPSR &(1 << SPIF)));
	
	//Simultaneous writing and reading is how SPI works. The received byte is in response to a previous sent byte, not the current.
	//After a read, the SPIF flag will be cleared automatically.
	data = SPDR;
	return data;
}

/*
A test function which can be used with logic analyzer tests
*/
void spiTest(){
	char lb = 0x00, hb = 0x00;
	//Initialize the SPI peripheral.
	spiSetMaster(SPI_CLOCK_16, SPI_POLARITY_0_PHASE_0, SPI_DATA_ORDER_MSB);
	//Set the CS pin.
	spiSetCSpin(SPI_CS_DDR_REGISTER_DEFAULT, DDB2);	
	//Set the CS pin high initially.
	spiCSHigh(SPI_CS_PORT_REGISTER_DEFAULT, PORTB2);
	//Set the CS pin low to start communication.
	spiCSLow(SPI_CS_PORT_REGISTER_DEFAULT, PORTB2);
	//Send a command + value.
	spiTransmitByte(0xA1);
	spiTransmitByte(0x33);
	//Receive the corresponding data by transmitting empty bytes.
	lb = spiTransmitByte(0xBB);
	hb = spiTransmitByte(0xCC);
	//Set the CS pin low to stop communication.
	spiCSHigh(SPI_CS_PORT_REGISTER_DEFAULT, PORTB2);
	//To avoid unused variable warnings.
	lb++, hb++;
}

