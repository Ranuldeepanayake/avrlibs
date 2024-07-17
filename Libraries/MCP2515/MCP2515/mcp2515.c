/*
 * MCP2515.c
 *
 * Created: 07-Jul-24 12:04:03 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
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
typedef struct rx_message{
	uint16_t message_id_h;
	uint16_t message_id_l; //For extended IDs.
	uint8_t id_type;
	uint8_t rtr_mode;
	uint8_t data_length;
	char data[8];
} rx_message;

volatile bool new_rx_message = false;
rx_message can_rx_message;

/*
ISR for Rx traffic. Uses the MCP2515 interrupt pin as the interrupt source.
*/
#if MCP2515_ISR_BANK==0
ISR(PCINT0_vect){
	//Read the interrupt flag and check which bank needs to be read. Reading will be done outside the ISR.
	if(1){
		new_rx_message = true;
	}
}
#elif MCP2515_ISR_BANK==1
ISR(PCINT1_vect){
	PORTC^= 0x01;
}
#elif MCP2515_ISR_BANK==2
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
	}else if(mode == MCP2515_MODE_LOOPBACK){
		//Do stuff.
	}
}

uint8_t mcp2515GetStatus(){
	//Reads the CANSTAT register. There is another register which can be used for this purpose.
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_CANSTAT);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
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
	
	//***Implement priority, EID capability and double check RTR.
	
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
	spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //Blank EID.
	spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //Blank EID.
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
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
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

void mcp2515UnsetInterrupts(){
	//Both Tx and Rx interrupts through OR arguments.
}

uint8_t mcp2515GetInterrupts(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_FLAGS);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

void mcp2515ClearInterrupts(){
	//Both Tx and Rx interrupts through OR arguments.
}

uint8_t mcp2515GetErrorFlags(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_ERROR_FLAGS);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return status;
}

uint8_t mcp2515GetTxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return count;
}


uint8_t mcp2515GetRxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return count;
}

/*
Receive functions.
*/

void mcp2515ReadReceiveBuffer(uint8_t buffer){
	//Zero out the struct.
	memset(&can_rx_message, 0x00, sizeof(can_rx_message));
	
	uint8_t id_type = 0, rtr_mode = 0, buffer_copy = 0, data_length = 0;
	buffer_copy = buffer; //To preserve the state of the buffer variable which will be modified later.
	 
	switch(buffer){
		case MCP2515_RECEIVE_BUFFER_0:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH;
			break;
	}
	
	//Check whether the message is an RTR or not.
	switch(buffer_copy){
		case MCP2515_RECEIVE_BUFFER_0:
			rtr_mode = mcp2515ReadByte(MCP2515_REGISTER_RXB0CTRL) & MCP2515_RECEIVE_RTR_YES;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			rtr_mode = mcp2515ReadByte(MCP2515_REGISTER_RXB1CTRL) & MCP2515_RECEIVE_RTR_YES;
			break;
	}
	can_rx_message.rtr_mode = rtr_mode;
	
	//Check whether the message ID is an SID or an EID.
	switch(buffer_copy){
		case MCP2515_RECEIVE_BUFFER_0:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB0SIDH) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB1SIDH) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
	}
	can_rx_message.id_type = id_type;
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Send a sequential read command.
	spiTransmitByte(buffer); //The Rxn CANINTF flag will automatically be cleared when a sequential read command is used. 
	//Get the ID.
	if(id_type == MCP2515_RECEIVE_STANDARD_ADDRESS){
		can_rx_message.message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) << 3); 
		can_rx_message.message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) >> 5);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}else if(id_type == MCP2515_RECEIVE_EXTENDED_ADDRESS){
		can_rx_message.message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) << 3);
		can_rx_message.message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) >> 5);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //***Need to do some shifting.
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}
	//Get the DLC.
	data_length = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	can_rx_message.data_length = data_length;
	//Get the data bytes according to the DLC.
	for(uint8_t bytes = 0; bytes < data_length; bytes++){
		can_rx_message.data[bytes] = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
}

uint8_t mcp2515ReadByte(uint8_t register_address){
	uint8_t byte = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(register_address);
	byte = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	return byte;
}

void mcp2515SetReceive(uint8_t buffer, uint8_t type, uint16_t message_id, uint16_t message_mask){
	//SID/EID, IDs, masks and etc.	
}

uint16_t mcp2515GetReceiveId(uint8_t buffer){
	return can_rx_message.message_id_h;
}

uint8_t mcp2515GetReceiveIdType(uint8_t buffer){
	return can_rx_message.id_type;
}

uint8_t mcp2515GetReceiveDataLength(uint8_t buffer){
	return can_rx_message.data_length;
}

uint8_t mcp2515GetReceiveRequestType(uint8_t buffer){
	return can_rx_message.rtr_mode;
}

char *mcp2515GetReceiveData(uint8_t buffer){
	return can_rx_message.data;
}

uint8_t mcp2515GetReceiveMessageHealth(uint8_t buffer){
	return 0;
	//Check CRC and return ok or not.
}
