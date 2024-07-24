/*
 * MCP2515.c
 *
 * Created: 07-Jul-24 12:04:03 AM
 * Author : Ranul Deepanayake
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "spi.h"
#include "mcp2515.h"

#ifdef MCP2515_DEBUG
#include "uart.h"
#endif

//***Move to progmem.
const uint8_t configuration_register_table[MCP2515_CONFIGURATION_REGISTER_TABLE_SIZE][MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH] = {
	{0x01,	0x9a,	0x07}, //0x03 //0x11 //0x03
	{0x01,	0xac,	0x03},
	{0x00,	0x00,	0x00},
	{0x00,	0x9a,	0x07},
	{0x00,	0x88,	0x03}
};

//Struct for Rx CAN frames.
typedef struct can_message{
	uint16_t message_id_h;
	uint16_t message_id_l; //For extended IDs.
	uint8_t id_type;
	uint8_t rtr_mode;
	uint8_t data_length;
	char data[8];
} can_message;

//Struct for the Rx buffer.
typedef struct can_buffer{
	volatile int head;
	volatile int tail;
	volatile int count;
	int size;
	volatile can_message message[MCP2515_CAN_BUFFER_SIZE];
} can_buffer;

//Buffer initialization.
can_buffer _can_buffer = { 
	.head = 0, .tail = 0, .count = 0, .size = MCP2515_CAN_BUFFER_SIZE,
	.message[0] = {
		.message_id_h = 0x0000,
		.message_id_l = 0x0000,
		.data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 }
	}, 
	.message[1] = {
		.message_id_h = 0x0000,
		.message_id_l = 0x0000,
		.data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 }
	},
	.message[2] = {
		.message_id_h = 0x0000,
		.message_id_l = 0x0000,
		.data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 }
	},
	.message[3] = {
		.message_id_h = 0x0000,
		.message_id_l = 0x0000,
		.data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 }
	}
};
can_message _popped_can_message = {
	.message_id_h = 0x0000,
	.message_id_l = 0x0000,
	.id_type = 0x00,
	.rtr_mode = 0x00,
	.data_length = 0x00,
	.data = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00 }
};

//Temporary char arrays for printing debug messages. Stores 16 bit numbers in hex.
char _debug_message_arr_16_b_1[7], _debug_message_arr_16_b_2[7];

//ISR vector and pin change interrupt bank selection. 
#if MCP2515_ISR_BANK == 0
	#define MCP2515_ISR_RX ISR(PCINT0_vect)
	#define MCP2515_ISR_PIN_REGISTER PINB
#elif MCP2515_ISR_BANK == 1
	#define MCP2515_ISR_RX ISR(PCINT1_vect)
	#define MCP2515_ISR_PIN_REGISTER PINC
#elif MCP2515_ISR_BANK == 2
	#define MCP2515_ISR_RX ISR(PCINT2_vect)
	#define MCP2515_ISR_PIN_REGISTER PIND
#endif

/*
ISR for Rx traffic. Uses the MCP2515 interrupt pin as the interrupt source.
*/
MCP2515_ISR_RX{
	//Read the interrupt flag and check which bank needs to be read. Reading will be done outside the ISR.
	//If the input pin is pulled low by the MCP2515.
	if((MCP2515_ISR_PIN_REGISTER & MCP2515_ISR_PIN)){
		return;
	}
	
	//PORTB |= 0x20; //Toggle the LED.
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	uint8_t interrupt = mcp2515GetInterrupts();
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	if((interrupt & MCP2515_INTERRUPT_FLAG_RX0) == MCP2515_INTERRUPT_FLAG_RX0){
		//Push frame into the Rx buffer. 
		mcp2515ReceiveBufferPush(MCP2515_RECEIVE_BUFFER_0);
		//Clear the MCP2515 interrupt flag.
		//mcp2515ClearInterrupts(interrupt & MCP2515_INTERRUPT_FLAG_RX0);
	}
	if((interrupt & MCP2515_INTERRUPT_FLAG_TX0) == MCP2515_INTERRUPT_FLAG_TX0){
		//Pop frame from the Tx buffer and transmit.
	}
	if((interrupt & MCP2515_INTERRUPT_FLAG_ERR) == MCP2515_INTERRUPT_FLAG_ERR){
		//Handle errors.
	}
	if((interrupt & MCP2515_INTERRUPT_FLAG_MERR) == MCP2515_INTERRUPT_FLAG_MERR){
		//Handle errors.
	}
	
	//PORTB &= ~0x20; //Toggle the LED.
}


//General functions.
void mcp2515Reset(){
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_RESET);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	uartPrintLn("[MCP2515][mcp2515Reset]::[Reset initiated]");
	#endif
}

void mcp2515Set(uint8_t mode, uint8_t speed){
	//Set up the three configuration registers.
	uint8_t conf_register[MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH] = {0x00, 0x00, 0x00};
	
	for(uint8_t register_number = 0; register_number < MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH; register_number++){
		conf_register[register_number] = configuration_register_table[speed][register_number];
	} 

	//Check canstat before changing the mode and switch to configuration mode if in any other mode. This has to be done in the main program since the mode cannot be changed while there 
	//are pending transmissions.
	
	
	//Assume that a reset has been issues and the device is already in configuration mnode.
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
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Write to the mode register.
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(MCP2515_REGISTER_CANCTRL);
	spiTransmitByte(mode);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	//Additional configuration.
	if(mode == MCP2515_MODE_CONFIG){
		//Do stuff.
	}else if(mode == MCP2515_MODE_NORMAL){
		//Do stuff.
	}else if(mode == MCP2515_MODE_LISTEN){
		//Do stuff.
	}else if(mode == MCP2515_MODE_LOOPBACK){
		//Do stuff.
	}else if(mode == MCP2515_MODE_SLEEP){
		//Do stuff.
	}
	
	//Initialize the structs.
	//mcp2515ClearStruct();
	
	#if MCP2515_DEBUG >= 2
	uint8_t status = mcp2515GetStatus();
	if((status & MCP2515_MODE_CONFIG) == MCP2515_MODE_CONFIG){uartPrintLn("[MCP2515][mcp2515Set]::[Mode][CONFIG]");}
	if((status & MCP2515_MODE_NORMAL) == MCP2515_MODE_NORMAL){uartPrintLn("[MCP2515][mcp2515Set]::[Mode][NORMAL]");}
	if((status & MCP2515_MODE_LISTEN) == MCP2515_MODE_LISTEN){uartPrintLn("[MCP2515][mcp2515Set]::[Mode][LISTEN]");}
	if((status & MCP2515_MODE_LOOPBACK) == MCP2515_MODE_LOOPBACK){uartPrintLn("[MCP2515][mcp2515Set]::[Mode][LOOPBACK]");}
	if((status & MCP2515_MODE_SLEEP) == MCP2515_MODE_SLEEP){uartPrintLn("[MCP2515][mcp2515Set]::[Mode][SLEEP]");}
	#endif
}

uint8_t mcp2515GetStatus(){
	//Reads the CANSTAT register. There is another register which can be used for this purpose.
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_CANSTAT);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 1
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(status, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetStatus]::[CANSTAT][");uartPrint(_debug_message_arr_16_b_1);uartPrintLn("]");
	#endif
		
	return status;
}

//Interrupt functions.
void mcp2515EnableInterrupts(uint8_t interrupts){
	//Set micro controller pin change interrupts.
	if(MCP2515_ISR_BANK == 0){
		PCICR |= 0x01; //Pin change interrupt bank enable.
		PCMSK0 |= MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}else if(MCP2515_ISR_BANK == 1){
		PCICR |= 0x02; //Pin change interrupt bank enable.
		PCMSK1 |= MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}else if(MCP2515_ISR_BANK == 2){
		PCICR |= 0x04; //Pin change interrupt bank enable.
		PCMSK2 |= MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}
	
	//Enable MCP2515 Tx and Rx interrupts through OR arguments.
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_ENABLE);
	spiTransmitByte(interrupts);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	//Enable micro controller global interrupts in case it was disabled somewhere else.
	sei();
	
	#if MCP2515_DEBUG >= 2
	uartPrint("[MCP2515][mcp2515EnableInterrupts]::[Interrupt enable command sent]"); uartPrintLn("");
	#endif
}

void mcp2515DisableInterrupts(uint8_t interrupts){
	//Set micro controller pin change interrupts.
	if(MCP2515_ISR_BANK == 0){
		PCICR &= ~0x01; //Pin change interrupt bank enable.
		PCMSK0 &= ~MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}else if(MCP2515_ISR_BANK == 1){
		PCICR &= ~0x02; //Pin change interrupt bank enable.
		PCMSK1 &= ~MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}else if(MCP2515_ISR_BANK == 2){
		PCICR &= ~0x04; //Pin change interrupt bank enable.
		PCMSK2 &= ~MCP2515_ISR_PIN; //Individual pin change interrupt pin enable.
	}
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_ENABLE);
	spiTransmitByte(~interrupts);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	//Do not disable global interrupts to prevent breaking other libraries.
	
	#if MCP2515_DEBUG >= 2
	uartPrint("[MCP2515][mcp2515DisableInterrupts]::[Interrupt disable command sent]"); uartPrintLn("");
	#endif
}

uint8_t mcp2515GetInterrupts(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_FLAGS);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(status, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetInterrupts]::[CANINTF][");uartPrint(_debug_message_arr_16_b_1);uartPrintLn("]");
	#endif
	
	return status;
}

//***Need a mechanism to only disable the required interrupt flag. 
void mcp2515ClearInterrupts(uint8_t interrupt){
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_BIT_MODIFY);
	spiTransmitByte(MCP2515_REGISTER_INTERRUPT_FLAGS);
	spiTransmitByte(interrupt); //Mask for bit modify.
	spiTransmitByte(interrupt);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	uartPrint("[MCP2515][mcp2515ClearInterrupts]::[Interrupt clear command sent]"); uartPrintLn("");
	#endif
}

//Error functions.
uint8_t mcp2515GetErrorFlags(){
	uint8_t status = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_ERROR_FLAGS);
	status = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 1
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(status, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetErrorFlags]::[EFLG][");uartPrint(_debug_message_arr_16_b_1);uartPrintLn("]");
	#endif
	
	return status;
}

uint8_t mcp2515GetTxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(count, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetTxErrorCount]::[TEC][");uartPrint(_debug_message_arr_16_b_1);uartPrintLn("]");

	#endif
	return count;
}

uint8_t mcp2515GetRxErrorCount(){
	uint8_t count = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(MCP2515_REGISTER_TX_ERROR_COUNT);
	count = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(count, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetRxErrorCount]::[REC][");uartPrint(_debug_message_arr_16_b_1);uartPrintLn("]");
	#endif
	
	return count;
}

//Transmit functions.
void mcp2515WriteByte(uint8_t register_address, char byte){
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(register_address);
	spiTransmitByte(byte);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_2));
	mcp2515DebugIntToHex(register_address, _debug_message_arr_16_b_1);
	mcp2515DebugIntToHex(register_address, _debug_message_arr_16_b_2);
	uartPrint("[MCP2515][mcp2515WriteByte]::[Register]["); uartPrint(_debug_message_arr_16_b_1); uartPrint("]["); uartPrint(_debug_message_arr_16_b_2); uartPrintLn("]");
	#endif
}

void mcp2515TransmitBufferWrite(uint8_t buffer, uint8_t address_mode, uint16_t address_h, uint16_t address_l,  uint8_t rtr_mode, uint8_t priority, char *data, uint8_t length){
	cli();
	
	switch(buffer){
		case MCP2515_TRANSMIT_BUFFER_0:
			buffer = MCP2515_COMMAND_WRITE_SEQUENTIAL_TXB0_SIDH;
			break;
	}
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(buffer);
	//Send the 11 bit SID.
	if(address_mode == MCP2515_TRANSMIT_STANDARD_ADDRESS){
		spiTransmitByte(address_h >> 3);	//SIDH.
		spiTransmitByte((address_h << 5) | address_mode); //SIDL.
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //Blank EID.
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //Blank EID.
	//Send the 29 bit EID.
	}else if(address_mode == MCP2515_TRANSMIT_EXTENDED_ADDRESS){
		spiTransmitByte(address_h >> 8);	//SIDH.
		spiTransmitByte(((address_h << 3) & 0xE0) | (address_h & 0x03) | address_mode); //SIDL.
		spiTransmitByte(address_l >> 8); //EIDH.
		spiTransmitByte(address_l); //EIDL.
	}
	
	//DLC.
	spiTransmitByte(rtr_mode | length);
	//Send the data bytes.
	for(uint8_t bytes = 0; bytes < length; bytes++){
		spiTransmitByte(*data++);
	}
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	//Set the transmit priority.
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_BIT_MODIFY);
	spiTransmitByte(0x03);
	spiTransmitByte(priority);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	sei();
}

void mcp2515Transmit(uint8_t buffer){
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
	spiTransmitByte(MCP2515_COMMAND_BIT_MODIFY);
	//spiTransmitByte(MCP2515_COMMAND_WRITE);
	spiTransmitByte(buffer);
	spiTransmitByte(MCP2515_TRANSMIT_TXBNCTRL_TRANSMIT); //Mask for bit modify.
	spiTransmitByte(MCP2515_TRANSMIT_TXBNCTRL_TRANSMIT);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	uartPrint("[MCP2515][mcp2515Transmit]::[Transmit command sent]"); uartPrintLn("");
	#endif
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
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	mcp2515DebugIntToHex(status, _debug_message_arr_16_b_1);
	uartPrint("[MCP2515][mcp2515GetTransmitStatus]::[TXBnCTRL]["); uartPrint(_debug_message_arr_16_b_1); uartPrintLn("]");
	#endif
	
	return status;
}

//Receive functions.
void mcp2515SetReceive(uint8_t buffer, uint8_t type, uint16_t message_id, uint16_t message_mask){
	//SID/EID, IDs, masks and etc.
}

uint8_t mcp2515ReceiveBufferPush(uint8_t buffer){
	//Disable interrupts since a data structure is being written to.
	cli();
	
	//Prevent overwriting when the buffer is full.
	if(_can_buffer.count ==  _can_buffer.size){
		return 2;
	}
	
	uint16_t message_id_h = 0x00, message_id_l = 0x00;
	uint8_t id_type = 0, rtr_mode = 0, buffer_copy = 0, data_length = 0, data[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, start_address = 0x00;
	buffer_copy = buffer; //To preserve the state of the buffer variable which will be modified later.
	
	switch(buffer){
		case MCP2515_RECEIVE_BUFFER_0:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH; //Should be buffer 1.
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
	_can_buffer.message[_can_buffer.head].rtr_mode = rtr_mode;
	
	//Check whether the message ID is an SID or an EID. ***Double check this.
	switch(buffer_copy){
		case MCP2515_RECEIVE_BUFFER_0:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB0SIDL) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB1SIDL) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
	}
	_can_buffer.message[_can_buffer.head].id_type = id_type;
	
	//Read byte by byte.
	start_address = MCP2515_REGISTER_RXB0SIDH;
	 
	//Get the SID.
	message_id_h |= mcp2515ReadByte(start_address++) << 3;
	message_id_h |= (mcp2515ReadByte(start_address) >> 5) | (mcp2515ReadByte(start_address++) & 0x03); //ORs the last two bits of the EID.
	//Get the EID.
	message_id_l |= mcp2515ReadByte(start_address++) << 8;
	message_id_l |= mcp2515ReadByte(start_address++);
	//Get the DLC.
	data_length = mcp2515ReadByte(start_address++) & 0x0F;	
	//Get the data bytes according to the DLC.
	for(uint8_t bytes = 0; bytes < data_length; bytes++){
		if(bytes >= 8){
			break;
		}
		data[bytes] = mcp2515ReadByte(start_address++);
	}
	
	
	//Insert the temporarily assigned data into the Rx buffer.
	_can_buffer.message[_can_buffer.head].message_id_h = message_id_h;
	_can_buffer.message[_can_buffer.head].message_id_l = message_id_l;
	_can_buffer.message[_can_buffer.head].data_length = data_length;
	for(uint8_t bytes = 0; bytes < data_length; bytes++){
		if(bytes >= 8){
			break;
		}
		_can_buffer.message[_can_buffer.head].data[bytes] = data[bytes];
	}
	
	//Increment buffer markers.
	_can_buffer.head++;
	_can_buffer.count++;
	
	//Link the head to the start of the buffer.
	if(_can_buffer.head == _can_buffer.size){
		_can_buffer.head = 0;
	}
	
	//Clear the MCP2515 interrupt flag to allow the next CAN frame to be received into the hardware buffer.
	mcp2515ClearInterrupts(MCP2515_INTERRUPT_FLAG_RX0);
		
	//Re-enable interrupts. Debug printing will not work correctly till interrupts are re-enabled.
	sei();
	
	#if MCP2515_DEBUG >= 1
	uartPrint("[MCP2515][mcp2515ReadReceiveBuffer]::");
	
	mcp2515DebugIntToHex(message_id_h, _debug_message_arr_16_b_1);
	uartPrint("IDH:"); uartPrint(_debug_message_arr_16_b_1);

	mcp2515DebugIntToHex(message_id_l, _debug_message_arr_16_b_1);
	uartPrint(" IDL:"); uartPrint(_debug_message_arr_16_b_1);
	
	if(id_type == MCP2515_RECEIVE_STANDARD_ADDRESS){
		uartPrint(" ID: SID");
	}else if(id_type == MCP2515_RECEIVE_EXTENDED_ADDRESS){
		uartPrint(" ID: EID");
	}else{
		uartPrint(" ID: ?");
	}
	
	if(rtr_mode == MCP2515_RECEIVE_RTR_YES){
		uartPrint(" RTR: Y");
	}else if(rtr_mode == MCP2515_RECEIVE_RTR_NO){
		uartPrint(" RTR: N");
	}else{
		uartPrint(" RTR: ?");
	}
	
	mcp2515DebugIntToHex(data_length, _debug_message_arr_16_b_1);
	uartPrint(" DLC:"); uartPrint(_debug_message_arr_16_b_1); uartPrint(" :: ");
	
	for(uint8_t i = 0; i < 8; i++){
		mcp2515DebugIntToHex(data[i], _debug_message_arr_16_b_1);
		uartPrint(_debug_message_arr_16_b_1); uartPrint(" ");
	}
	uartPrintLn("");
	
	#endif
	
	return 1;
	
	/*
	//Disable interrupts since a data structure is being written to.
	cli();
	
	//Prevent overwriting when the buffer is full.
	if(_can_buffer.count ==  _can_buffer.size){
		return 2;
	}
	
	uint8_t id_type = 0, rtr_mode = 0, buffer_copy = 0, data_length = 0;
	buffer_copy = buffer; //To preserve the state of the buffer variable which will be modified later.
	 
	switch(buffer){
		case MCP2515_RECEIVE_BUFFER_0:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			buffer = MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH; //Should be buffer 1.
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
	_can_buffer.message[_can_buffer.head].rtr_mode = rtr_mode;
	
	//Check whether the message ID is an SID or an EID. ***Double check this.
	switch(buffer_copy){
		case MCP2515_RECEIVE_BUFFER_0:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB0SIDH) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
		case MCP2515_RECEIVE_BUFFER_1:
			id_type = mcp2515ReadByte(MCP2515_REGISTER_RXB1SIDH) & MCP2515_RECEIVE_EXTENDED_ADDRESS;
			break;
	}
	_can_buffer.message[_can_buffer.head].id_type = id_type;
	
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	//Send a sequential read command.
	spiTransmitByte(buffer); //The Rxn CANINTF flag will automatically be cleared when a sequential read command is used. 
	//Get the ID.
	if(id_type == MCP2515_RECEIVE_STANDARD_ADDRESS){
		_can_buffer.message[_can_buffer.head].message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) << 3); 
		_can_buffer.message[_can_buffer.head].message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) >> 5);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}else if(id_type == MCP2515_RECEIVE_EXTENDED_ADDRESS){
		_can_buffer.message[_can_buffer.head].message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) << 3);
		_can_buffer.message[_can_buffer.head].message_id_h |= (spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE) >> 5);
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE); //Need to do some shifting.
		spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}
	//Get the DLC.
	data_length = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	_can_buffer.message[_can_buffer.head].data_length = data_length;
	//Get the data bytes according to the DLC.
	for(uint8_t bytes = 0; bytes < data_length; bytes++){
		_can_buffer.message[_can_buffer.head].data[bytes] = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	}
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	//Increment buffer markers.
	_can_buffer.head++;
	_can_buffer.count++;
	
	//Link the head to the start of the buffer.
	if(_can_buffer.head == _can_buffer.size){
		_can_buffer.head = 0;
	}
	
	//Clear the MCP2515 interrupt flag to allow the next CAN frame to be received into the hardware buffer.
	//Need to implement the bit modify command.
	uint8_t interrupt = mcp2515GetInterrupts();
	mcp2515ClearInterrupts(interrupt & MCP2515_INTERRUPT_FLAG_RX0);
		
	//Re-enable interrupts. Debug printing will not work correctly till interrupts are re-enabled.
	sei();
	
	#if MCP2515_DEBUG >= 1
	uartPrint("[MCP2515][mcp2515ReadReceiveBuffer]::");
	
	mcp2515DebugIntToHex(_can_buffer.message[_can_buffer.head - 1].message_id_h, _debug_message_arr_16_b_1);
	uartPrint("IDH:"); uartPrint(_debug_message_arr_16_b_1);

	mcp2515DebugIntToHex(_can_buffer.message[_can_buffer.head - 1].message_id_l, _debug_message_arr_16_b_1);
	uartPrint(" IDL:"); uartPrint(_debug_message_arr_16_b_1);
	
	if(_can_buffer.message[_can_buffer.head - 1].id_type == MCP2515_RECEIVE_STANDARD_ADDRESS){
		uartPrint(" ID: SID");
	}else if(_can_buffer.message[_can_buffer.head - 1].id_type == MCP2515_RECEIVE_EXTENDED_ADDRESS){
		uartPrint(" ID: EID");
	}else{
		uartPrint(" ID: ?");
	}
	
	if(_can_buffer.message[_can_buffer.head - 1].rtr_mode == MCP2515_RECEIVE_RTR_YES){
		uartPrint(" RTR: Y");
	}else if(_can_buffer.message[_can_buffer.head - 1].rtr_mode == MCP2515_RECEIVE_RTR_NO){
		uartPrint(" RTR: N");
	}else{
		uartPrint(" RTR: ?");
	}
	
	mcp2515DebugIntToHex(_can_buffer.message[_can_buffer.head - 1].data_length, _debug_message_arr_16_b_1);
	uartPrint(" DLC:"); uartPrint(_debug_message_arr_16_b_1); uartPrint(" :: ");
	
	for(uint8_t i = 0; i < 8; i++){
		mcp2515DebugIntToHex(_can_buffer.message[_can_buffer.head - 1].data[i], _debug_message_arr_16_b_1);
		uartPrint(_debug_message_arr_16_b_1); uartPrint(" ");
	}
	uartPrintLn("");
	
	#endif
	
	return 1;
	
	*/
}

uint8_t mcp2515ReceiveBufferPop(uint8_t buffer){
	//Check for an empty buffer.
	if(_can_buffer.count == 0){
		return 3;
	}
	
	//_popped_can_message = _can_buffer.message[_can_buffer.tail];
	//memcpy(&_popped_can_message, &_can_buffer.message[_can_buffer.tail], sizeof(can_message));
	_popped_can_message.message_id_h = _can_buffer.message[_can_buffer.tail].message_id_h;
	_popped_can_message.message_id_l = _can_buffer.message[_can_buffer.tail].message_id_l;
	_popped_can_message.id_type = _can_buffer.message[_can_buffer.tail].id_type;
	
	_can_buffer.tail++;
	_can_buffer.count--;
	
	//Link the tail to the start of the buffer.
	if(_can_buffer.tail == _can_buffer.size){
		_can_buffer.tail = 0;
	}
	return 1;
}

uint8_t mcp2515Read(){
	return mcp2515ReceiveBufferPop(MCP2515_RECEIVE_BUFFER_0);
}

uint8_t mcp2515ReadAvailable(){
	if(_can_buffer.count > 0){
		return 1;
	}else{
		return 0;
	}
}

uint8_t mcp2515ReadCount(){
	return _can_buffer.count;
}

uint8_t mcp2515ReadIsFull(){
	if(_can_buffer.count == MCP2515_CAN_BUFFER_SIZE){
		return 1;
	}else{
		return 0;
	}
}

uint8_t mcp2515ReadPeek(){
	if(mcp2515ReadAvailable()){
		_popped_can_message = _can_buffer.message[_can_buffer.tail];
		return 1;
	}else{
		return 0;
	}
}

uint8_t mcp2515ReadByte(uint8_t register_address){
	uint8_t byte = 0x00;
	spiCSLow(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	spiTransmitByte(MCP2515_COMMAND_READ);
	spiTransmitByte(register_address);
	byte = spiTransmitByte(MCP2515_COMMAND_DUMMY_BYTE);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, MCP2515_CS_PIN);
	
	#if MCP2515_DEBUG >= 2
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_1));
	memset(_debug_message_arr_16_b_1, 0, sizeof(_debug_message_arr_16_b_2));
	mcp2515DebugIntToHex(register_address, _debug_message_arr_16_b_1);
	mcp2515DebugIntToHex(byte, _debug_message_arr_16_b_2);
	uartPrint("[MCP2515][mcp2515ReadByte]::[Register]["); uartPrint(_debug_message_arr_16_b_1); uartPrint("]["); uartPrint(_debug_message_arr_16_b_2); uartPrintLn("]");
	#endif
	
	return byte;
}

uint16_t mcp2515GetReceiveId(){
	return _popped_can_message.message_id_h;
}

uint8_t mcp2515GetReceiveIdType(){
	return _popped_can_message.id_type;
}

uint8_t mcp2515GetReceiveDataLength(){
	return _popped_can_message.data_length;
}

uint8_t mcp2515GetReceiveRequestType(){
	return _popped_can_message.rtr_mode;
}

char *mcp2515GetReceiveData(){
	return _popped_can_message.data;
}

uint8_t mcp2515GetReceiveMessageHealth(){
	return 0;
	//Check CRC and return ok or not.
}

//Debug functions.
void mcp2515DebugIntToHex(uint16_t input, char *temp_1){
	char temp_2[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	memset(temp_1, 0, sizeof(&temp_1));
	//memset(temp2, 0, STRING_LENGTH);
	//sprintf(temp2, "%x", input);
	itoa(input, temp_2, 16);
	if(input < 16) {
		strcat(temp_1, "0x0");
		strcat(temp_1, temp_2);
		//uartPrint("0x0"); uartPrint(temp2);
	}else{
		strcat(temp_1, "0x");
		strcat(temp_1, temp_2);
		//uartPrint("0x"); uartPrint(temp2);
	}
}

void mcp2515ClearStruct(){
	//memset(&_can_buffer, 0, sizeof(_can_buffer));
	_can_buffer = (struct can_buffer) {
	};
}
