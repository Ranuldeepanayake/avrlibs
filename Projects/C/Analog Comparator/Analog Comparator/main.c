/*
 * Analog Comparator.c
 *
 * Created: 14-Dec-18 11:59:15 AM
 * Author : Ranul Deepanayake
 * Demonstrates the use of the analog comparator.
 */ 

#include <avr/io.h>

void setComparator(); 

int main(void)
{
	setComparator();
	
    while (1) 
    {
		if(ACSR & 0x20){	//Check the ACO flag.
			PORTB|= 0x20;
		}
		else{
			PORTB&= ~0x20;
		}
    }
}

void setComparator(){
	ACSR= 0x40;		//Internal bandgap reference voltage set as positive input.
	DIDR1= 0x03;	//Disable the digital input buffer for power saving.
	DDRB= 0x20;		//LED for testing.
}

