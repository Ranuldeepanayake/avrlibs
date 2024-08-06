/*
 * Interrupt Vector Table.c
 *
 * Created: 02-Dec-18 9:14:15 PM
 * Author : Ranul Deepanayake
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
    sei();	//Enable global interrupts.
	
    while (1) 
    {
    }
}

void setPins(){
	DDRD= 0x12;	//Set pin direction (set pin 1 as output for UART TX).
	DDRC= 0x01; //Set LED output.
	PORTD= 0x10; //Enable pull-up resistor on PORTD4 (to use a push-button).
	
	PCICR= 0x04; //Enable pin change interrupts on port D.
	PCMSK2= 0x10;	//Enable external pin interrupts on PORTD4.
}


