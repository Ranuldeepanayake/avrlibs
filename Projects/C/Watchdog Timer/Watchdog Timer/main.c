/*
 * Watchdog Timer.c
 *
 * Created: 04-Dec-18 10:15:54 AM
 * Author : Ranul Deepanayake
 * Demonstrates the use of the watchdog timer in reset mode. 
 */ 

#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void setWatchdog();
void setLed();

int main(void)
{
	setWatchdog();
	setLed();
	
    while (1) 
    {
		_delay_ms(250);	//Turn on the LED.
		PORTB= 0x20;
		wdt_reset();	//Reset the WDT before the WDT timeout to prevent a system reset. 
    }
}

void setWatchdog(){
	//The below steps are extremely important when changing the WDT settings. Please refer the datasheet.
	//WDTCSR|= 0x80;	//Clear the WDT interrupt flag (write a one).
	//MCUSR&= ~0x08;	//Clear the WDT system reset flag (write a zero).
	cli();	//Disable global interrupts during the below timed cycle.
	WDTCSR= 0x18;	//Write a one to WDCE and WDE (regardless of WDE's current status). The next step must be executed within four cycles of this step before WDCE automatically clears.
	WDTCSR= 0x0D; //Enable WDT MCU reset and set WDT timeout for 2S. WDCE will automatically be cleared.
	sei();	//Re-enable global interrupts (if required).
}

void setLed(){
	DDRB= 0x20;	//Set the LED.
}




