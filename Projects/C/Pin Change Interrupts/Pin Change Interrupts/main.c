/*
 * Pin Change Interrupts.c
 *
 * Created: 02-Dec-18 1:31:26 PM
 * Author : Ranul Deepanayake
 * Demonstrates the use of pin change interrupts.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void setPins();

ISR(PCINT2_vect){
	PORTC^= 0x01;
}

int main(void)
{
	setPins();
	//sei();	//Enable this if using the ISR.
	
    while (1) 
    {
		//Manually serviced the interrupt.
		if(PCIFR & 0x04){
			PORTC^= 0x01;
			PCIFR|= 0x04;
		}
    }
}

void setPins(){
	DDRD= 0x12;	//Set pin direction (set pin 1 as output for UART TX).
	DDRC= 0x01; //Set LED output.
	PORTD= 0x10; //Enable pull-up resistor on PORTD4 (to use a push-button).
	
	PCICR= 0x04; //Enable pin change interrupts on port D.
	PCMSK2= 0x10;	//Enable external pin interrupts on PORTD4.
}

