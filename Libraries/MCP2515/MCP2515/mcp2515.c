/*
 * MCP2515.c
 *
 * Created: 07-Jul-24 12:04:03 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "mcp2515.h"

//***Move to progmem.
const uint8_t configuration_register_table[MCP2515_CONFIGURATION_REGISTER_TABLE_SIZE][MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH] = {
	{0x01,	0x9a,	0x07}, //0x03 //0x11 //0x03
	{0x01,	0xac,	0x03},
	{0x00,	0x00,	0x00},
	{0x00,	0x9a,	0x07},
	{0x00,	0x88,	0x03}
};

//Struct for Rx messages.
struct rx_message{
	uint16_t message_id_h = 0x00;
	uint16_t message_id_l = 0x00; //For extended IDs.
	uint8_t id_type = 0x00;
	uint8_t rtr_mode = 0x00;
	uint8_t data_length = 0x00;
	uint8_t data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};

volatile rx_message *message;

/*
ISR for Rx traffic. Uses the MCP2515 interrupt pin as the interrupt source.
*/
#if MCP2515_ISR_BANK 0
ISR(PCINT0_vect){
	PORTC^= 0x01;
}
#elif MCP2515_ISR_BANK 1
ISR(PCINT1_vect){
	PORTC^= 0x01;
}
#elif MCP2515_ISR_BANK 2
ISR(PCINT2_vect){
	PORTC^= 0x01;
}
#endif

void mcp2515Reset(){
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_RESET);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

void mcp2515Set(uint8_t mode, uint8_t speed){
	//Set up the three configuration registers.
	uint8_t conf_register[MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH] = {0x00, 0x00, 0x00};
	
	for(uint8_t register_number = 0; register_number < MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH; register_number++){
		conf_register[register_number] = configuration_register_table[speed][register_number];
	} 

	//Check canstat before changing the mode and switch to configuration mode if in any other mode.
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Write to the mode register.
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(MCP2515_REGISTER_CANCTRL);
	spiTransmitByte(mode);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Write to the CNF registers sequentially.
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(MCP2515_REGISTER_CNF3);
	spiTransmitByte(conf_register[2]);
	spiTransmitByte(MCP2515_REGISTER_CNF2);
	spiTransmitByte(conf_register[1]);
	spiTransmitByte(MCP2515_REGISTER_CNF3);
	spiTransmitByte(conf_register[0]);	
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	if(mode == MCP2515_MODE_CONFIGURATION){
		//Do stuff.
	}else if(mode == MCP2515_MODE_NORMAL){
		//Do stuff.
	}else if(mode == MCP2515_MODE_LISTEN){
		//Do stuff.
	}
	
	//Normal, listen and loopback etc.
	
	//Set CNF0-2
	//CANCTL:3
	//TXRTSCTRL:
	
	//Check canstat whether the actual mode is actually set.
}

uint8_t mcp2515GetStatus(){
	//Reads the CANSTAT register. There is another register which can be used for this purpose.
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_CANSTAT);
	status = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

void mcp2515WriteByte(uint8_t register_address, char byte){
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(register_address);
	spiTransmitByte(byte);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

void mcp2515LoadTransmitBuffer(uint8_t buffer, uint8_t buffer_priority, uint16_t address, uint8_t address_mode, uint8_t rtr_mode, char *data, uint8_t length){
	
	//***Implement priority and EID.
	switch(buffer){
		case MCP2515_TRANSMIT_BUFFER_0:
			buffer = MCP2515_COMMAND_WRITE_SEQUENTIAL_TXB0_SIDH;
			break;
	}
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Send the 11 bit SID.
	spiTransmitByte(buffer);
	spiTransmitByte(address >> 3);	//SIDH.
	spiTransmitByte((address << 5) | address_mode); //SIDL.
	spiTransmitByte(0x00); //Blank EID.
	spiTransmitByte(0x00); //Blank EID.
	//DLC.
	spiTransmitByte(rtr_mode | length);
	//Send the data bytes.
	for(uint8_t bytes = 0; bytes < length; bytes++){
		spiTransmitByte(*data++);
	}
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

uint8_t mcp2515GetTransmitStatus(uint8_t buffer){
	//Reads the TXBnCTRL register.
	uint8_t status = 0x00;
	switch(buffer){
		case MCP2515_TRANSMIT_BUFFER_0:
			buffer = MCP2515_REGISTER_TXB0CTRL;
			break;
		case MCP2515_TRANSMIT_BUFFER_1:
			buffer = MCP2515_REGISTER_TXB1CTRL;
			break;
		case MCP2515_TRANSMIT_BUFFER_2:
			buffer = MCP2515_REGISTER_TXB1CTRL;
			break; 
	}
		
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(buffer);
	status = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

void mcp2515TransmitNow(uint8_t buffer){
	//***Read the current buffer priority and send it since it will be overwritten, if necessary.
	switch(buffer){
		case MCP2515_TRANSMIT_BUFFER_0:
			buffer = MCP2515_REGISTER_TXB0CTRL;
			break;
		case MCP2515_TRANSMIT_BUFFER_1:
			buffer = MCP2515_REGISTER_TXB1CTRL;
			break;
		case MCP2515_TRANSMIT_BUFFER_2:
			buffer = MCP2515_REGISTER_TXB1CTRL;
			break;
	}
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(buffer);
	spiTransmitByte(MCP2515_TRANSMIT_TXBNCTRL_TRANSMIT);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

void mcp2515SetInterrupts(){
	//Both Tx and Rx interrupts through OR arguments.
}

uint8_t mcp2515GetInterrupts(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_FLAGS);
	status = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

uint8_t mcp2515GetErrorFlags(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_ERROR_FLAGS);
	status = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

uint8_t mcp2515GetTxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return count;
}


uint8_t mcp2515GetRxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(0xFF);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return count;
}

/*
Receive functions.
*/

void mcp2515ReadData(uint8_t buffer, uint16_t *data, uint8_t data_length){
	
	//***Implement buffer selection and priority.
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Send the 11 bit SID.
	spiTransmitByte(MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH);
	spiTransmitByte(address >> 3);	//SIDH.
	spiTransmitByte((address << 5) | address_mode); //SIDL.
	spiTransmitByte(0x00); //Blank EID.
	spiTransmitByte(0x00); //Blank EID.
	//DLC.
	spiTransmitByte(rtr_mode | length);
	//Send the data bytes.
	for(uint8_t bytes = 0; bytes < length; bytes++){
		spiTransmitByte(*data++);
	}
	
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

uint8_t mcp2515ReadByte(uint8_t _register_address){
	uint8_t byte = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(_register_address);
	spiTransmitByte(_register_address);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return byte;
}

void mcp2515SetReceive(uint8_t buffer, uint8_t type, uint16_t message_id, uint16_t message_mask){
	//SID/EID, IDs, masks and etc.	
}

uint8_t mcp2515GetReceiveId(uint8_t buffer, uint8_t id_type, uint16_t *message_id){

}

uint8_t mcp2515GetReceiveIdType(uint8_t buffer, uint8_t *id_type){
	//Read SIDL.
}

uint8_t mcp2515GetReceiveDataLength(uint8_t buffer, uint8_t *length){
	//Read DLC.
}

uint8_t mcp2515GetReceiveRequestType(uint8_t buffer, uint8_t *request_type){
	//Read SIDL if SID or DLC if EID. Must be conditional based on the ID type.
}

uint8_t mcp2515GetReceiveData(uint8_t buffer, uint8_t *request_type){
	//Read SIDL if SID or DLC if EID. Must be conditional based on the ID type.
}

uint8_t mcp2515GetReceiveMessageHealth(){
	//Check CRC and return ok or not.
}
