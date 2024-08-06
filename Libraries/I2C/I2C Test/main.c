/*
 * I2C.c
 *
 * Created: 31-Oct-18 11:56:35 AM
 * Author : Ranul Deepanayake
 *
 */ 

#include <avr/io.h>
#include "i2c.h"
#include "uart.h"
#include "i2cscan.h"

int main(void)
{
	uartSet(UART_BAUD_RATE(9600), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);
	i2cSetMaster(I2C_PRESCALER, I2C_BAUD_RATE);
	uartPrintLn("Init>");
	i2cScan();
	
    while (1) 
    {	
    }
}



