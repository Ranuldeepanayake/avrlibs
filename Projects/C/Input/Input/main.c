/*
 * Input.c
 *
 * Created: 02-Oct-18 9:59:20 AM
 * Author : Ranul Deepanayake
 * Demonstrates analog and digital inputs.	
 */ 

#include "header.h"

int main(void)
{
	//DDRB|= 0x20; //Use this if using the test led.
	setDigitalInputs();
	//setDigitalInputsSamePort();
	
    while (1) 
    {
		//testLed();
		//buttonStatus(readPins());
		//buttonStatusInverted(readPins());
		buttonStatusSamePort(readPins());
    }
}


