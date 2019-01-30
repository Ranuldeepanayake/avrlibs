/*
 * ADC.c
 *
 * Created: 29-Jan-19 5:29:37 PM
 * Author : Ranul Deepanayake
 */ 

#include <util/delay.h>
#include <stdio.h>
#include "adc.h"
#include "uart.h"

int main(void)
{
	uint16_t value= 0;
	char temp[5];
	adc_set(ADC_AREF_AVCC, ADC_INTERRUPT_ENABLE, ADC_PRESCALER_128);
	uart_set(UART_BAUD_RATE(9600), 8, UART_PARITY_NONE, UART_STOP_BITS_1);
    
    while (1) 
    {
		value= adc_read_temperature_sensor();
		sprintf(temp, "%d", value);
		uart_print("ADC value: "), uart_print(temp), uart_println(" C");
		_delay_ms(250), _delay_ms(250);
    }
}

