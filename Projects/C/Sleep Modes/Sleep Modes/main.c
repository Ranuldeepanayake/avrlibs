/*
 * Sleep Modes.c
 *
 * Created: 02-Dec-18 7:14:12 PM
 * Author : Ranul Deepanayake
 * Demonstrates the use of sleep modes with interrupts.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void setPins();
void setSleep();
void blink();

ISR(PCINT2_vect){
	//The ISR body must be declared for some reason.
}

int main(void)
{
    setPins();
	setSleep();
	sei();	//Enable global interrupts.
	
    while (1) 
    {   
		//blink();
		PORTC|= 0x01;
		sleep_cpu(); //'sleep' in assembly.
		PORTC&= ~0x01;
		sleep_cpu(); //'sleep' in assembly.
    }
}

void setPins(){
	DDRC= 0x01; //Set LED.
	DDRD= 0x02; //Set input pin and UART TX.
	PCICR= 0x04; //Enable pin change interrupts on PORTD.
	PCMSK2= 0x10; //Enable in change interrupts on PORTD4. 
}

void setSleep(){
	SMCR= 0x05; //Set sleep mode (power down is selected) and enable sleep.
}


