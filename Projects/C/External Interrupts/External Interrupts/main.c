/*
 * External Interrupts.c
 *
 * Created: 02-Dec-18 12:13:20 PM
 * Author : Ranul Deepanayake
 * Demonstrates the use of external interrupt pins.
 * ***Use a 0.1 mFd capacitor or software de-bouncing if using a switch.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void setPins();

ISR(INT0_vect){
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
	//DDRD= 0x02;	//Set pin direction (set pin 1 as output for UART TX).
	DDRC= 0x01; //Set LED output.
	PORTD= 0x0C; //Enable pull-up resistors on both EXT INT pins. *Important if using a switch and trigger on low level.
	
	EICRA= 0x02; //Select low level, logical change, falling edge or rising edge on INT pin (falling edge used).
	EIMSK= 0x03;	//Enable external pin interrupts.
}

