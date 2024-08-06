/*
 * MCP2515 Test.c
 *
 * Created: 07-Jul-24 12:04:38 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "spi.h"
#include "mcp2515.h"
#include "uart.h"
#include <util/delay.h>

#define STRING_LENGTH 7

char obd2_pid_5[8] = {0x03, 0x01, 0x05, 0x23, 0x55, 0x55, 0x55, 0x55}; //OBD frame for a temperature response.
const uint16_t obd2_can_sid_ecu = 0x7E8;

void messageDebugTx(char *message, char *temp){
	memset(temp, 0, sizeof(&temp));

	uartPrint("CAN message sent to buffer>"); uartPrint(" [ ");
	for(uint8_t i = 0; i < 8; i++){
		sprintf(temp, "%x", message[i]);
		uartPrint("0x"); uartPrint(temp); uartPrint(", ");
	}
	uartPrint("] "); uartPrintLn("");
}

int main(void)
{
	//char text[STRING_LENGTH];
	uartSet(UART_BAUD_RATE(250000), 8, UART_PARITY_NONE, UART_STOP_BITS_1);
	
	spiSetMaster(SPI_CLOCK_128, SPI_POLARITY_0_PHASE_0, SPI_DATA_ORDER_MSB);
	spiSetCSpin(MCP2515_CS_DDR_REGISTER, DDB2);
	spiCSHigh(MCP2515_CS_PORT_REGISTER, PORTB2);
	
	mcp2515Reset();
	mcp2515Set(MCP2515_MODE_NORMAL, MCP2515_CONF_8MHZ_50SP_125KBPS);
	//mcp2515EnableInterrupts(MCP2515_INTERRUPT_ENABLE_RX0);
	
	uartPrintLn("Initialization done >");
	
    while (1) 
    {	
		//***Convert Tx into interrupt based.
		if(!((mcp2515GetTransmitStatus(MCP2515_TRANSMIT_BUFFER_0) & MCP2515_TRANSMIT_TXB0CTRL_STATUS_PENDING) == MCP2515_TRANSMIT_TXB0CTRL_STATUS_PENDING)){
			mcp2515TransmitBufferWrite(MCP2515_TRANSMIT_BUFFER_0, MCP2515_TRANSMIT_STANDARD_ADDRESS, obd2_can_sid_ecu, 0x0000, MCP2515_TRANSMIT_RTR_NO, MCP2515_TRANSMIT_PRIORITY_HIGHEST, obd2_pid_5, 8);
			//messageDebugTx(obd2_pid_5, text);
			mcp2515Transmit(MCP2515_TRANSMIT_BUFFER_0);
		}
		
		if((mcp2515GetInterrupts() & MCP2515_INTERRUPT_FLAG_RX0) == MCP2515_INTERRUPT_FLAG_RX0){
			mcp2515ReceiveBufferPush(MCP2515_RECEIVE_BUFFER_0);
		}

		//Do Rx stuff.
		//while(mcp2515ReadAvailable()){
			////After the read function, all variables need to be taken action on before the next read function loads another frame.
			//mcp2515Read();
			//mcp2515GetReceiveId();
			//mcp2515GetReceiveIdType();
		//}
		//mcp2515ReceiveBufferPush(MCP2515_RECEIVE_BUFFER_0);
    }
}

/*
Testing code.

mcp2515DebugIntToHex(mcp2515GetReceiveId(MCP2515_RECEIVE_BUFFER_0), text);
uartPrint("Rx status [3] [Rx received] [Message ID]: "); uartPrintLn(text);

//mcp2515DebugIntToHex(mcp2515GetReceiveId(MCP2515_RECEIVE_BUFFER_0), text);
//uartPrint("Rx status [3] [Rx received] [Message ID]: "); uartPrintLn(text);
//
//mcp2515DebugIntToHex(mcp2515GetReceiveIdType(MCP2515_RECEIVE_BUFFER_0), text);
//uartPrint("Rx status [3] [Rx received] [ID type]: "); uartPrintLn(text);

//mcp2515DebugIntToHex(mcp2515GetReceiveRequestType(MCP2515_RECEIVE_BUFFER_0), text);
//uartPrint("Rx status [3] [Rx received] [Request type]: "); uartPrintLn(text);

//mcp2515DebugIntToHex(mcp2515GetReceiveDataLength(MCP2515_RECEIVE_BUFFER_0), text);
//uartPrint("Rx status [3] [Rx received] [Data length]: "); uartPrintLn(text);

//memset(text, 0, sizeof(text));
//char *data_ptr = mcp2515GetReceiveData(MCP2515_RECEIVE_BUFFER_0);
//for(uint8_t i = 0; i < 8; i++){
////itoa(*(data_ptr + i), text, 16);
//mcp2515DebugIntToHex(*(data_ptr + i), text);
//uartPrint(text); uartPrint(" ");
//}
//uartPrintLn("  : End of Rx data");

*/

