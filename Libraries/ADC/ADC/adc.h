/*
 * adc.h
 *
 * Created: 29-Jan-19 5:30:05 PM
 * Author: Ranul Deepanayake
 * ADC library for the ATmega328P. Uses single conversion mode.
 * Supports 6 channel and 8 channel package models.
 * Supports selectable AREF source.
 * Supports the inbuilt temperature sensor.
 * Uses right adjusted ADC readings.
 */ 


#ifndef ADC_H_
#define ADC_H_

//Includes.
#include <avr/io.h>

//Debug level.
#ifndef ADC_DEBUG
	#define ADC_DEBUG 2
#endif
//ADC pin numbers.
#define ADC_0 0
#define ADC_1 1
#define ADC_2 2
#define ADC_3 3
#define ADC_4 4
#define ADC_5 5
#if ADC_PACKAGE == ADC_PACKAGE_PDIP
	#define ADC_TEMP 6
	#define ADC_BGP 7
	#define ADC_GND 8
#elif ADC_PACKAGE != ADC_PACKAGE_PDIP
	#define ADC_6 6
	#define ADC_7 7
	#define ADC_TEMP 8
	#define ADC_BGP 9
	#define ADC_GND 10
#endif
//Physical package type. Needed to set the appropriate number of channels.
#define ADC_PACKAGE_PDIP 0
#define ADC_PACKAGE_TQFP 1
#define ADC_PACKAGE_VFQFN 2
#define ADC_PACKAGE ADC_PACKAGE_PDIP
//Channel count (depends on the physical package).
#if ADC_PACKAGE == ADC_PACKAGE_PDIP
	#define ADC_CHANNEL_COUNT 0x09
#elif ADC_PACKAGE != ADC_PACKAGE_PDIP
	#define ADC_CHANNEL_COUNT 0x0B
#endif
//Channel definition.
#define ADC_CHANNEL_0 0x00
#define ADC_CHANNEL_1 0x01
#define ADC_CHANNEL_2 0x02
#define ADC_CHANNEL_3 0x03
#define ADC_CHANNEL_4 0x04
#define ADC_CHANNEL_5 0x05
#define ADC_CHANNEL_TEMPERATURE_SENSOR 0x08
#define ADC_CHANNEL_BANDGAP 0x0E
#define ADC_CHANNEL_GROUND 0x0F
//Other packages will have two additional channels.
#if ADC_PACKAGE != ADC_PACKAGE_PDIP
	#define ADC_CHANNEL_6 0x06
	#define ADC_CHANNEL_7 0x07
#endif
//Voltage reference sources.
#define ADC_VREF_EXTERNAL 0x00
#define ADC_VREF_AVCC 0x40
#define ADC_VREF_INTERNAL 0xC0
//Prescaler values.
#define ADC_PRESCALER_2 0x01
#define ADC_PRESCALER_4 0x02
#define ADC_PRESCALER_8 0x03
#define ADC_PRESCALER_16 0x04
#define ADC_PRESCALER_32 0x05
#define ADC_PRESCALER_64 0x06
#define ADC_PRESCALER_128 0x07 //Most suitable for free running mode to reduce the number of interrupts.
//ADC modes.
#define ADC_MODE_SINGLE_CONVERSION 0
#define ADC_MODE_FREE_RUNNING 1
//Options.
#define ADC_TRIGGER_FREE_RUNNING 0x00
#define ADC_INTERRUPT_ENABLE 0x08
#define ADC_INTERRUPT_DISABLE 0x00
#define ADC_CHANNEL_UNSET 0x0F

//Functions.
//Set up the ADC peripheral.
void adcSet(uint8_t mode, uint8_t channel, uint8_t prescaler, uint8_t vref);
//Return a 10 bit ADC reading in free-running mode.
uint16_t adcRead(uint8_t channel);
//Return a 10 bit ADC reading in single conversion mode.
uint16_t adcReadWait(uint8_t channel);
//Return the temperature from the internal temperature sensor (in Celcius).
uint16_t adcReadTemperatureSensor();
//Disable the digital input buffer of a channel for power saving.
void adcDisableDigitalInputBuffer(uint8_t channel);
//Enable the digital input buffer of a channel.
void adcEnableDigitalInputBuffer(uint8_t channel);
void adcDebugIntToHex(uint16_t input, char *temp_1);

/*

Example implementation.

int main(void)
{
	adcSet(ADC_MODE_FREE_RUNNING, ADC_CHANNEL_0, ADC_PRESCALER_128, ADC_VREF_AVCC);
	adcDisableDigitalInputBuffer(0x02); //Optional for power saving.
	
	while (1)
	{
		uint8_t = adcRead(ADC_CHANNEL_1);
	}
}

*/

#endif /* ADC_H_ */