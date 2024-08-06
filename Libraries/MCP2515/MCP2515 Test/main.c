/*
 * MCP2515 Test.c
 *
 * Created: 07-Jul-24 12:04:38 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include "spi.h"

int main(void)
{
    char lb = 0x00, hb = 0x00;
    //Initialize the SPI peripheral.
    spiSetMaster(SPI_CLOCK_4, SPI_POLARITY_0_PHASE_0, SPI_DATA_ORDER_MSB);
    //Set the CS pin.
    spiSetCSpin(DDRB, DDB2);
    //Set the CS pin high initially.
    spiCSHigh(PORTB, PORTB2);
    //Set the CS pin low to start communication.
    spiCSLow(PORTB, PORTB2);
    //Send a command + value.
    spiTransmitByte(0xA0);
    //Receive the corresponding data by transmitting empty bytes.
    lb = spiTransmitByte(0x00);
    //Set the CS pin low to stop communication.
    spiCSHigh(PORTB, PORTB2);
    //To avoid unused variable warnings.
    lb++, hb++;
	
    while (1) 
    {
    }
}

