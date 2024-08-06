/*
 * Integration 1.c
 *
 * Created: 10-Oct-18 7:38:43 PM
 * Author : Ranul Deepanayake
 * Integrates ADCs, Timers and UART communication.
 */ 

#include "header.h"

volatile uint16_t potentiometerValue= 0;
volatile uint8_t adcLowByte= 0;
char generalString[10]; //Space for the maximum value+ null character;

ISR(ADC_vect){
	adcLowByte= ADCL;
	potentiometerValue= ((ADCH<< 8) | (adcLowByte));
}

int main(void)
{
	setLed();
    setAdc();
	setTimer1();
	setUart();
	
	
    while (1) 
    {
		writePwm();
		debugCode();
		_delay_ms(50);
    }
}

void setLed(){
	DDRD|= 0x42;
}

void setAdc(){
	ADMUX|= 0x45; //Set AREF to Vcc and multiplexer to channel 5.
	ADCSRA|= 0xAF; //Enable ADC, enable ADC interrupts and set pre-scaler.
	DIDR0|= 0xFF; //Disable digital input buffer for power saving.
	sei(); //Enable global interrupts.
	ADCSRA|= 0x40; //Start conversion. 
}

void setTimer1(){
	TCCR0A|= 0x83; //Set compare output mode and waveform generation.
	TCCR0B|= 0x03; //Set prescaler.		
} 

void writePwm(){
	if(TCNT0>= 9){
		OCR0A= potentiometerValue>> 4;	//Divided by 4 to get in the range of 8 bits (1024/ 4= 256)//(int) map(potentiometerValue, 0, 1023, 0, 255);
		TCNT0&= 0x00;
	}
}

void setUart(){
	UCSR0A&= ~(0x02); //Unset 2x rate.
	UCSR0B|= 0x18; //Enable RX & TX;
	UCSR0C|= 0x06; //Set data length.
	UBRR0H= (uint8_t) (BAUDPRESCALER>> 8); //Set baudrate high byte.
	UBRR0L= (uint8_t) BAUDPRESCALER; //Set baudrate low byte.
}

void sendChar(unsigned char data){
	while(!(UCSR0A & 0x20));
	UDR0= data;
}

void sendString(char *string){
	while(*string!= 0x00){
		sendChar(*string);
		string++;
	}
}

void returnValue(){
	//uint16_t modA= 50000% 1024;
	//uint16_t divA= 50000/ 1024;
	//uint16_t major= divA* potentiometerValue;
	//uint16_t minor= (modA* potentiometerValue)/ 10;
	//uint16_t whole= major+ minor;
	//uint8_t returnMajor= 0;
	//uint16_t returnMinor= 0;
	
	//returnMajor= whole/ 10000;
	//returnMinor= whole% 10000; 
}

float readVoltage(){
	return (5.0/1024.0)* potentiometerValue;
}

float map (float value, float inMin, float inMax, float outMin, float outMax){
	return (value- inMin)* (outMax- outMin)/ (inMax- outMin)+ outMin;
}

void debugCode(){
	//itoa(potentiometerValue, generalString, 10); //Converts bytes to ASCII strings.
	sprintf(generalString, "%d", potentiometerValue);
	sendString("Potentiometer value: ");
	sendString(generalString);
	
	sendString("	");
	
	//snprintf(potentiometerValueString, sizeof(potentiometerValueString), "%d.%d", 12, 5486);
	dtostrf(readVoltage(), 7, 3, generalString);
	sendString("Potentiometer voltage: ");
	sendString(generalString);
	
	sendChar(CARRIAGE_RETURN);
	sendChar(NEWLINE);
}
