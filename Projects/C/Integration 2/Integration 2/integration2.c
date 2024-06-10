/*
 * integration2.c
 *
 * Created: 24-Dec-18 1:18:32 PM
 *  Author: ranul
 */ 
#include "integration2.h"

void setADC(){
	//Set the ADC to work in free-running interrupt mode.
	ADCSRA= 0xAF;		//Enable ADC, set ADATE, ADIE and pre-scaler.
	ADCSRB= 0x00;		//Set free running mode.
	ADMUX= 0x40;
	DIDR0= 0xFF;		//Disable digital input buffer for power saving.
	ADCSRA|= 0x40;		//Start conversion.
}

