/*
 * mcp2515.h
 *
 * Created: 09-Jul-24 7:17:30 PM
 * Author: ranul
 */ 

#include <stdbool.h>

#ifndef MCP2515_H_
#define MCP2515_H_

#define MCP2515_DEBUG 1

//Default port registers and the pin for SPI.
#ifndef MCP2515_CS_DDR_REGISTER
#define MCP2515_CS_DDR_REGISTER 0x04
#endif

#ifndef MCP2515_CS_PORT_REGISTER
#define MCP2515_CS_PORT_REGISTER 0x05
#endif

#ifndef MCP2515_CS_PIN
#define MCP2515_CS_PIN PORTB2
#endif

//Pin change interrupt bank.
#ifndef MCP2515_ISR_BANK
#define MCP2515_ISR_BANK 2 //0: PORTB, 1: PORTC, 2: PORTD
#endif

//Pin change interrupt pin.
#ifndef MCP2515_ISR_PIN
#define MCP2515_ISR_PIN 0x10
#endif

//CAN Rx buffer size in 13 byte CAN frames.
#define MCP2515_CAN_BUFFER_SIZE 8

/*
SPI commands.
*/
#define MCP2515_COMMAND_RESET 0xC0
#define MCP2515_COMMAND_WRITE 0x02
#define MCP2515_COMMAND_BIT_MODIFY 0x05
#define MCP2515_COMMAND_WRITE_SEQUENTIAL_TXB0_SIDH 0x40
#define MCP2515_COMMAND_WRITE_SEQUENTIAL_TXB0_D0 0x41
#define MCP2515_COMMAND_RTS_TXB0 0x81
#define MCP2515_COMMAND_READ 0x03
#define MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_SIDH 0x90
#define MCP2515_COMMAND_READ_SEQUENTIAL_RXB0_D0 0x92
#define MCP2515_COMMAND_DUMMY_BYTE 0x00

//Timing configuration selector.
#define MCP2515_CONF_8MHZ_50SP_125KBPS		0
#define MCP2515_CONF_8MHZ_75SP_125KBPS		1
#define MCP2515_CONF_8MHZ_50SP_500KBPS		2
#define MCP2515_CONF_16MHZ_50SP_500KBPS		3
#define MCP2515_CONF_16MHZ_50SP_1000KBPS	4
#define MCP2515_CONFIGURATION_REGISTER_TABLE_SIZE 5
#define MCP2515_CONFIGURATION_REGISTER_TABLE_DEPTH 3

/*
Operation modes.
*/
#define MCP2515_MODE_CONFIG 0x87
#define MCP2515_MODE_LISTEN 0x67
#define MCP2515_MODE_NORMAL 0x07
#define MCP2515_MODE_LOOPBACK 0x47
#define MCP2515_MODE_SLEEP 0x27

/*
Registers.
*/
//Common.
#define MCP2515_REGISTER_CNF1 0x2A
#define MCP2515_REGISTER_CNF2 0x29 
#define MCP2515_REGISTER_CNF3 0x28 
#define MCP2515_REGISTER_CANSTAT 0x0E
#define MCP2515_REGISTER_CANCTRL 0x0F
#define MCP2515_REGISTER_TX_ERROR_COUNT 0x1C
#define MCP2515_REGISTER_RX_ERROR_COUNT 0x1D
#define MCP2515_REGISTER_ERROR_FLAGS 0x2D
#define MCP2515_REGISTER_INTERRUPT_ENABLE 0x2B
#define MCP2515_REGISTER_INTERRUPT_FLAGS 0x2C
//Write.
#define MCP2515_REGISTER_TXB0CTRL 0x30
#define MCP2515_REGISTER_TXB1CTRL 0x40
#define MCP2515_REGISTER_TXB2CTRL 0x50
//Read.
#define MCP2515_REGISTER_RXB0CTRL 0x60
#define MCP2515_REGISTER_RXB0SIDH 0x61
#define MCP2515_REGISTER_RXB0SIDL 0x62
#define MCP2515_REGISTER_RXB0EIDH 0x63
#define MCP2515_REGISTER_RXB0EIDL 0x64
#define MCP2515_REGISTER_RXB0DLC 0x65
#define MCP2515_REGISTER_RXB1CTRL 0x70
#define MCP2515_REGISTER_RXB1SIDH 0x71
#define MCP2515_REGISTER_RXB1SIDL 0x72
#define MCP2515_REGISTER_RXB1EIDH 0x73
#define MCP2515_REGISTER_RXB1EIDL 0x74
#define MCP2515_REGISTER_RXB1DLC 0x75

/*
Flags.
*/
//Write.
#define MCP2515_TRANSMIT_BUFFER_0 0
#define MCP2515_TRANSMIT_BUFFER_1 1
#define MCP2515_TRANSMIT_BUFFER_2 2
#define MCP2515_TRANSMIT_STANDARD_ADDRESS	0x00 
#define MCP2515_TRANSMIT_EXTENDED_ADDRESS	0x08
#define MCP2515_TRANSMIT_RTR_NO						0x00
#define MCP2515_TRANSMIT_RTR_YES					0x40
#define MCP2515_TRANSMIT_PRIORITY_HIGHEST			0x03
#define MCP2515_TRANSMIT_PRIORITY_LOWEST			0x00
#define MCP2515_TRANSMIT_TXBNCTRL_TRANSMIT			0x08
#define MCP2515_TRANSMIT_TXB0CTRL_STATUS_PENDING	0x08
//Read.
#define MCP2515_RECEIVE_BUFFER_0 0
#define MCP2515_RECEIVE_BUFFER_1 1
#define MCP2515_RECEIVE_STANDARD_ADDRESS 0x00
#define MCP2515_RECEIVE_EXTENDED_ADDRESS 0x08
#define MCP2515_RECEIVE_RTR_NO 0x00
#define MCP2515_RECEIVE_RTR_YES 0x08
//Interrupts.
#define MCP2515_INTERRUPT_ENABLE_RX0	0x01
#define MCP2515_INTERRUPT_ENABLE_TX0	0x04
#define MCP2515_INTERRUPT_ENABLE_ERR	0x20
#define MCP2515_INTERRUPT_ENABLE_MERR	0x80
#define MCP2515_INTERRUPT_FLAG_RX0	0x01
#define MCP2515_INTERRUPT_FLAG_TX0	0x04
#define MCP2515_INTERRUPT_FLAG_ERR	0x20
#define MCP2515_INTERRUPT_FLAG_MERR	0x80



/*
Function declaration.
*/
void mcp2515Reset();
void mcp2515Set(uint8_t mode, uint8_t speed);
uint8_t mcp2515GetStatus();

void mcp2515EnableInterrupts(uint8_t interrupts);
void mcp2515DisableInterrupts(uint8_t interrupts);
uint8_t mcp2515GetInterrupts();
void mcp2515ClearInterrupts(uint8_t interrupt);

uint8_t mcp2515GetErrorFlags();
uint8_t mcp2515GetTxErrorCount();
uint8_t mcp2515GetRxErrorCount();

void mcp2515TransmitBufferWrite(uint8_t buffer, uint8_t address_mode, uint16_t address_h, uint16_t address_l,  uint8_t rtr_mode, uint8_t priority, char *data, uint8_t length);
void mcp2515Transmit(uint8_t buffer);
void mcp2515WriteByte(uint8_t address, char byte);
uint8_t mcp2515GetTransmitStatus(uint8_t buffer);

void mcp2515SetReceive(uint8_t buffer, uint8_t type, uint16_t message_id, uint16_t message_mask);
uint8_t mcp2515ReceiveBufferPush(uint8_t buffer);
uint8_t mcp2515ReceiveBufferPop(uint8_t buffer);
uint8_t mcp2515Read();
uint8_t mcp2515ReadAvailable();
uint8_t mcp2515ReadByte(uint8_t register_address);
uint16_t mcp2515GetReceiveId();
uint8_t mcp2515GetReceiveIdType();
uint8_t mcp2515GetReceiveDataLength();
uint8_t mcp2515GetReceiveRequestType();
char *mcp2515GetReceiveData();
uint8_t mcp2515GetReceiveMessageHealth();

void mcp2515DebugIntToHex(uint16_t input, char *temp_1);
void mcp2515ClearStruct();

#endif /* MCP2515_H_ */