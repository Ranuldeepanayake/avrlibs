/*
 * HC-SR04 Ultrasonic Sensor.c
 *
 * Created: 22-Nov-18 8:47:30 PM
 * Author : Ranul
 * Send a logic high square wave for 10 microseconds.
 * Capture the echo using a pin change interrupt or digital input. Look for a logic high signal and count the number of microseconds o f the signal.
 * Use a pin change interrupt to read the echo.*********************
 * Convert delay into distance.
 */ 
#include "hcsr04.h"

volatile uint16_t microseconds= 0;
volatile uint8_t state= 0;
double distance= 0;

ISR(TIMER1_COMPA_vect){
	microseconds++;
	state= 1;	//Switch to make sure echo is read only after a trigger signal.
}

int main(void)
{
    hcsr04SetTrigger();
	setUsart(9600);
	DDRD|= 0x02;	//Set pin 7 as input.
	DDRB|= 0x02;	//Set pin 2 as output.
	
    while (1) 
    {
		hcsr04ReadEcho();
    }
}


void hcsr04SetTrigger(){
	TCCR1A= 0x40; //OCR1A toggle on match and CTC mode.
	TCCR1B= 0xC9; //Input capture ES and NC, CTC mode, TOP on OCR1A and clock pre-scaler: 1.
	OCR1A= 78;	//1MHz: 7, 100KHz: 78.
	TIMSK1= 0x02; //Enable OCR1A interrupt.
	TCNT1= 0;	//Initialize Timer 1,
	sei();	//Enable global interrupts.
}

void hcsr04ReadEcho(){
	if(getState()== 1){
		char text[10];
		uint16_t echo_start_time= 0, echo_end_time= 0, echo_duration= 0;
		
		if(PIND & 0x80){
			state= 0;
			return;	//Skip reading if signal had been already started.
		}
		while(!(PIND & 0x80));	//Wait till the echo starts.
		echo_start_time= getMicroseconds();
		while(PIND & 0x80);	//Wait till the whole echo is read.
		echo_end_time= getMicroseconds();
		
		echo_duration= echo_end_time- echo_start_time;
		distance= ((echo_duration* 0.034)/2);	//Add an offset maybe?
		
		state= 0;
		
		dtostrf(distance, 6, 2, text);
		println(text);
	}
	
}

uint16_t getMicroseconds(){
	cli();
	uint16_t temp_microseconds= microseconds;
	sei();
	return temp_microseconds;
}

uint8_t getState(){
	cli();
	uint8_t temp_state= state;
	sei();
	return temp_state;
}