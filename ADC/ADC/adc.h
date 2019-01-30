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

//Attributes.
#define ADC_PACKAGE_PDIP 0
#define ADC_PACKAGE_TQFP 1
#define ADC_PACKAGE_VFQFN 2
#define ADC_PACKAGE ADC_PACKAGE_PDIP

#define ADC_AREF_EXTERNAL 0x00
#define ADC_AREF_AVCC 0x40
#define ADC_AREF_INTERNAL_BANDGAP 0xC0
#define ADC_INTERRUPT_ENABLE 0x08
#define ADC_INTERRUPT_DISABLE 0x00
#define ADC_PRESCALER_2 0x01
#define ADC_PRESCALER_4 0x02
#define ADC_PRESCALER_8 0x03
#define ADC_PRESCALER_16 0x04
#define ADC_PRESCALER_32 0x05
#define ADC_PRESCALER_64 0x06
#define ADC_PRESCALER_128 0x07
#define ADC_CHANNEL_0 0x00
#define ADC_CHANNEL_1 0x01
#define ADC_CHANNEL_2 0x02
#define ADC_CHANNEL_3 0x03
#define ADC_CHANNEL_4 0x04
#define ADC_CHANNEL_5 0x05
#define ADC_CHANNEL_TEMPERATURE_SENSOR 0x08
#define ADC_CHANNEL_BANDGAP 0x0E
#define ADC_CHANNEL_GROUND 0x0F
#if ADC_PACKAGE!= ADC_PACKAGE_PDIP
#define ADC_CHANNEL_6 0x06
#define ADC_CHANNEL_7 0x07
#endif
#define ADC_CHANNEL_DESELECT 0xF0

//Status and error codes.
#define ADC_SINGLE_CONVERSION_PENDING 0x40

//Functions.
//Set up the ADC peripheral.
void adc_set(uint8_t reference_voltage, uint8_t interrupt_enable, uint8_t prescaler);
//Return a 10 bit ADC reading in single conversion mode.
uint16_t adc_read(uint8_t channel);
//Disable the digital input buffer of a channel for power saving.
void adc_disable_digital_buffer(uint8_t channel);
//Return the temperature from the internal temperature sensor (in Celcius).
uint16_t adc_read_temperature_sensor();

#endif /* ADC_H_ */