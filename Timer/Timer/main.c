/*
 * Timer.c
 *
 * Created: 29-Jan-19 4:06:38 PM
 * Author : Ranul Deepanayake
 */ 

#include <avr/io.h>
#include "timer.h"

int main(void)
{
	uint16_t previous_time= 0, current_time= 0, on_time= 50, total_time= 100;
	timer_set_micros();
	DDRB|= 0x20;
	
    while (1) 
    {
		current_time= timer_get_micros();
		if((current_time- previous_time)>= total_time){
			PORTB|= 0x20;
			previous_time= timer_get_micros();
		}
		
		if(current_time- previous_time>= on_time){
			PORTB&= ~(0x20);
		}
    }
}

