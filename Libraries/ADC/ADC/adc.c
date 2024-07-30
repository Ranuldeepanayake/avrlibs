/*
 * adc.h
 *
 * Created: 29-Jan-19 5:30:05 PM
 * Author: Ranul Deepanayake
 * ADC library for the ATmega328P. 
 * Uses the single conversion mode in a round-robin manner without interrupts.
 * Supports 10 bit resolution.
 * Supports 6 channel and 8 channel packages.
 * Supports a selectable VREF source.
 * Supports the inbuilt temperature sensor, band gap reference and ground auxiliary channels.
 * Uses right adjusted ADC readings.
 * Supports digital pin input buffer disabling for power saving.
 * No buffers or smoothening functions are used.
 * Supports debugging over UART.
 */ 

#include "adc.h"
#ifdef ADC_DEBUG
	#include <stdlib.h>
	#include <string.h>
	#include "uart.h"
#endif


//Array initialization based on the number of channels present.
#if ADC_PACKAGE == ADC_PACKAGE_PDIP
	volatile uint16_t _adc_results[ADC_CHANNEL_COUNT] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Initialize the array.
#elif ADC_PACKAGE != ADC_PACKAGE_PDIP
	volatile uint16_t _adc_results[ADC_CHANNEL_COUNT] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Initialize the array.
#endif

//Global variables.
volatile uint8_t _adc_channel_index = ADC_CHANNEL_0, _adc_vref_temp = 0;
uint8_t _adc_mode = ADC_MODE_SINGLE_CONVERSION;

/*
Set the ADC peripheral. Selectable mode, prescaler and vref source.
The single conversion mode starts by initially reading ADC channel 0. Subsequent channels are read by adcRead() in a round robin manner.
Currently supports the single conversion mode only.
*/
void adcSet(uint8_t mode, uint8_t prescaler, uint8_t vref){
	//Save the vref value to a global variable to be used within the ISR for safely changing the MUX bits.
	_adc_vref_temp = vref;
	
	if(mode == ADC_MODE_SINGLE_CONVERSION){
		//Single conversion mode.
		_adc_mode = ADC_MODE_SINGLE_CONVERSION;
		ADMUX |= (vref | ADC_CHANNEL_0); //Select the voltage reference and channel 0 for the initial reading.
		ADCSRA |= ((1 << ADEN) | prescaler); //Enable the ADC, enable triggers, set pre-scalers and enable interrupts.
		ADCSRA |= (1 << ADSC); //Start free running conversion. If ADSC is enabled at the same time as ADEN, it will take 25 ADC cycles instead of 13.
		
		#if ADC_DEBUG >= 1
			uartPrint("[ADC][adcSet][mode][Single conversion]");
		#endif
		
	}else if(mode == ADC_MODE_FREE_RUNNING){
		_adc_mode = ADC_MODE_FREE_RUNNING;
		//Do stuff.
	}
}

/*
Return a 10 bit ADC reading in the single conversion mode. Reads all ADC channels with each call to this function in a round robin manner. 
ADC channel readings are stored in an array with one element per channel. Not recommended to be used with 'adcReadWait()'.
*/
uint16_t adcRead(uint8_t channel){
	//If no conversions are pending.
	if(!(ADCSRA & (1 << ADSC))){

		_adc_results[_adc_channel_index] = ADCW; //Read the current reading.
		
		//Increment the channel index to read the next ADC channel during the next call to this function.
		_adc_channel_index++;
		
		//Roll over to channel 0.
		if(_adc_channel_index == ADC_CHANNEL_COUNT){
			_adc_channel_index = ADC_CHANNEL_0;
		}
		
		//Conditional checker to support various package types will more or less ADC channels.
		#if ADC_PACKAGE == ADC_PACKAGE_PDIP
			//Set the new channel. The following conditional checker is needed since ADC channel numbers are not contiguous from channel 6.
			if(_adc_channel_index <= ADC_CHANNEL_5){
				ADMUX = (_adc_vref_temp | _adc_channel_index);
				
			}else if(_adc_channel_index > ADC_CHANNEL_5){
				switch (_adc_channel_index){
					case 6:
					//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_TEMPERATURE_SENSOR);
						break;
					case 7:
						//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_BANDGAP);
						break;
					case 8:
						//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_GROUND);
						break;
				}
			}
			
		#elif ADC_PACKAGE != ADC_PACKAGE_PDIP
			//Set the new channel. The following conditional checker is needed since ADC channel numbers are not contiguous from channel 6.
			if(_adc_channel_index <= ADC_CHANNEL_7){
				ADMUX = (_adc_vref_temp | _adc_channel_index);
				
			}else if(_adc_channel_index > ADC_CHANNEL_7){
				switch (_adc_channel_index){
					case 8:
						//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_TEMPERATURE_SENSOR);
						break;
					case 9:
						//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_BANDGAP);
						break;
					case 10:
						//Unset the previous channel.
						ADMUX = (_adc_vref_temp | ADC_CHANNEL_GROUND);
						break;
				}
			}
		#endif
		
		ADCSRA |= (1 << ADSC); //Restart conversion.
	}
	
	#if ADC_DEBUG >= 1
		char temp[5], temp_2[5];
		adcDebugIntToHex(channel, temp); adcDebugIntToHex(_adc_results[channel], temp_2);
		uartPrint("[ADC][read][CH]["); uartPrint(temp); uartPrint("]["); uartPrint(temp_2); uartPrintLn("]");
	#endif
	
	return _adc_results[channel];
}


/*
Returns a 10 bit ADC reading in single conversion mode but through polling. Only reads the channel specified in the argument.
A delay of (1 / ADC frequency) * 13 will be introduced when polling. Not recommended to be used with 'adcRead()'.
*/
uint16_t adcReadWait(uint8_t channel){
	//Do not change the channel if a conversion is in progress.
	if(ADCSRA & (1<< ADSC)){
		return 0;
	}
	
	ADMUX&= ~ADC_CHANNEL_UNSET; //Clear previously set channels.
	
	ADMUX |= channel; //Set the channel.
	ADCSRA |= (1<< ADSC); //Start single conversion.
	while(ADCSRA & (1<< ADSC)); //Wait for conversion to complete (for bit to become 0).
	
	//Read the value.
	uint16_t value = ADCW;
	
	#if ADC_DEBUG >= 1
		char temp[5];
		adcDebugIntToHex(value, temp);
		uartPrint("[ADC][readWait][val]["); uartPrint(temp); uartPrintLn("]");
	#endif
	
	return value;
}

/*
Returns the value of the internal temperature sensor in Celcius. Calls the 'adcRead()' function with the temperature sensor channel. 
Uses a formula for the temperature calculation. Calibration might be needed for a more accurate reading. 
*/
uint16_t adcReadTemperatureSensor(){
	uint16_t temperature = 0;
	
	uint16_t raw_value = adcRead(ADC_TEMP);

	//Temperature calculation in Celcius.
	temperature = ((uint16_t)((raw_value- 242)* (85- (-45))/ (380- 242)+ (-45)))- 35;
	
	#if ADC_DEBUG >= 1
		char temp[5];
		adcDebugIntToHex(temperature, temp);
		uartPrint("[ADC][readTempSensor][C]["); uartPrint(temp); uartPrintLn("]");
	#endif
	
	return temperature;
}

/*
Disable the digital input buffer of an ADC channel for power saving (not required).
*/
void adcDisableDigitalInputBuffer(uint8_t channel){
	DIDR0 |= channel; //Write a 1 to disable.
}

/*
Enable the digital input buffer of an ADC channel if it was disabled for power saving.
*/
void adcEnableDigitalInputBuffer(uint8_t channel){
	DIDR0 &= ~channel; //Write a 0 to enable.
}

/*
Supporting function which converts numbers in the decimal format to hexadecimal format when storing as a string.
*/
void adcDebugIntToHex(uint16_t input, char *temp_1){
	memset(temp_1, 0, sizeof(&temp_1));
	itoa(input, temp_1, 10);
}




