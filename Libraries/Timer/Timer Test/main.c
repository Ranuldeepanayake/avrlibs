/*
 * main.c
 *
 * Created: 31-Jul-24 10:54:47 PM
 *  Author: ranul
 */ 

#include <avr/io.h>
#include "timer.h"
#include "adc.h"

int main(void)
{	
	//adcSet(ADC_MODE_SINGLE_CONVERSION, ADC_PRESCALER_128, ADC_VREF_AVCC);
	//timerSetOutputPins(TIMER_2, TIMER_N_CHANNEL_B);
	//timer2Write(TIMER_N_CHANNEL_A, 0);
	//timer2Set(TIMER_N_MODE_CTC, TIMER_N_PIN_B_TOGGLE, TIMER_N_PRESCALER_64);
	
	uint16_t previous_time= 0, current_time= 0, total_time= 5;
	DDRD |= 0x80;
	
	timer2SetMillis();
	
	while (1)
	{
		//timer2Write(TIMER_N_CHANNEL_B, (uint8_t) (adcRead(ADC_0) / 4));
		
		current_time= timer2GetMillis();
		if((current_time - previous_time) >= total_time){
			PORTD ^= 0x80;
			previous_time = timer2GetMillis();
		}
		//PORTD ^= 0x80;
	}
}