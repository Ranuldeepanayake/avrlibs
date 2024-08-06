/*
 * UART Test.c
 *
 * Created: 17-Jun-24 11:40:46 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"


int main(void)
{
    /* Replace with your application code */
	uartSet(UART_BAUD_RATE(115200), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);	//Set up UART peripheral.
	//uartSetLed(1);
    while (1) 
    { 
		if(!uartAvailable(UART_BUFFER_TYPE_TX)){
			uartPrint("This is the new library ");  uartPrintLn("Hello world!");
		}
		//_delay_us(1000);
    }
}

