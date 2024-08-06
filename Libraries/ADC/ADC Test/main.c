/*
 * AnalogRead.c
 *
 * Created: 02-Oct-18 6:02:54 PM
 * Author : Ranul Deepanayake
 * Demonstrates ADC reading.
 *
 *--------------------------------------------------------------------------------------------------------------------------------------------
 *														ADC SIngle Conversion Mode
 *--------------------------------------------------------------------------------------------------------------------------------------------
 *
 * Step 1: Configure the ADC clock (use the pre-scaler to divide main clock). ADC uses 50Khz- 200Khz. Divisors are 2, 4, 8, 16, 32, 64 & 128.
		 Ex- If system clock is 16Mhz, divide it by 128 to get a 125Khz clock for the ADC.
 * Step 2: Configure AREF (ADC reference voltage).	Use 5V for an Arduino Uno.	
 * Step 2: Set ADLAR (ADC Left Adjust Result) to 1.	??
 * Step 3: Enable the ADC with ADEN.
 * Step 4: Select the MUX channel (only one can be set and read at a time).
 * Step 5: ADSC has to be set to 1 to start a conversion. 
 * Step 6: ADSC is automatically set to 0 after a conversion is completed. Check for this condition.
 * Step 7: Read the 10 bit (unless set to 8 bit) value using ADCW.	  
 */ 

#include <avr/io.h>
#include "adc.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>


int main(void)
{	
	adcSet(ADC_MODE_SINGLE_CONVERSION, ADC_PRESCALER_128, ADC_VREF_INTERNAL);
	uartSet(UART_BAUD_RATE(250000), UART_DATA_SIZE_8, UART_PARITY_NONE, UART_STOP_BITS_1);
	adcDisableDigitalInputBuffer(0xFF);
	char temp[6];
	
    while (1) 
    {	
		//memset(temp, 0, sizeof(temp));
		//itoa(adcReadTemperatureSensor(), temp, 10);
		//uartPrintLn(temp);
		//adcRead(ADC_CHANNEL_1);
		//adcRead(6);
		//adcReadTemperatureSensor();
		adcRead(ADC_TEMP);
		adcRead(ADC_CHANNEL_BANDGAP);
		//adcRead(ADC_CHANNEL_BANDGAP);
		//adcRead(ADC_CHANNEL_GROUND);
		_delay_ms(50);
    }
}

