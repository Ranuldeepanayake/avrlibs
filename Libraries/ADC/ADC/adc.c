/*
 * adc.c
 *
 * Created: 29-Jan-19 5:29:52 PM
 * Author: Ranul Deepanayake
 */ 

#include "adc.h"

/*
Set up the ADC peripheral.
*/
void adc_set(uint8_t reference_voltage, uint8_t interrupt_enable, uint8_t prescaler){
	ADMUX|= reference_voltage;
	ADCSRA|= ((1<< ADEN) | interrupt_enable | prescaler); //Enable the ADC and set pre-scaler.
}

/*
Return a 10 bit ADC reading in single conversion mode.
*/
uint16_t adc_read(uint8_t channel){
	//Single conversion mode.
	adc_disable_digital_buffer(channel);
	ADMUX&= ADC_CHANNEL_DESELECT; //Clear previously set channels.
	ADMUX|= channel; //Set the channel.
	ADCSRA|= ADC_SINGLE_CONVERSION_PENDING; //Start single conversion.
	while(ADCSRA & ADC_SINGLE_CONVERSION_PENDING); //Wait for conversion to complete (for bit to become 0).
	
	uint16_t value= ADCL;	//Read low byte first.
	value|= (ADCH<< 8);
	return value;
}

/*
Disable the digital input buffer of a channel for power saving (not required).
*/
void adc_disable_digital_buffer(uint8_t channel){
	DIDR0&= ~channel;
}

uint16_t adc_read_temperature_sensor(){
	uint16_t raw_value= 0;
	
	adc_set(ADC_AREF_INTERNAL_BANDGAP, ADC_INTERRUPT_DISABLE, ADC_PRESCALER_128);
	raw_value= adc_read(ADC_CHANNEL_TEMPERATURE_SENSOR);
	
	return ((uint16_t)((raw_value- 242)* (85- (-45))/ (380- 242)+ (-45)))- 35;
}

