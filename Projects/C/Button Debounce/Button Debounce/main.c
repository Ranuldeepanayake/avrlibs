/*
 * Button Debounce.c
 *
 * Created: 04-Dec-18 2:31:44 PM
 * Author : Ranul Deepanayake
 * Demonstrates button debouncing.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define check(PORT) if(PORT & 0x01) blinkLed()

void setTimer();
void setButton();
void setLed();
void ledOn();
void ledOff();
uint16_t getMilliseconds();

volatile uint16_t milliseconds= 0;

ISR(TIMER0_COMPA_vect){
	milliseconds++;
}

uint8_t flip= 1;
uint8_t switch_pressed= 0;
uint8_t led_button_flag= 0;
uint16_t new_time= 0;
uint16_t previous_time= 0;

int main(void)
{
    setTimer();
	sei();
	setButton();
	setLed();
	
    while (1) 
    {
		//check(PINC);
		//blinkLed();
		/*if(!(PINC & 0x01) && flip== 1){
			blinkLed();
		}
		if((PINC & 0x01) && flip== 1){
			//blinkLed();
			flip= 0;
		}
		if(!(PINC & 0x01) && flip== 0){
			blinkLed();
		}
		if((PINC & 0x01) && flip== 0){
			flip= 1;
		}*/
		if(!(PINC & 0x01) && switch_pressed== 0){
			previous_time= getMilliseconds();
			switch_pressed= 1;
		}
		if((PINC & 0x01) && switch_pressed== 1 && (getMilliseconds()- previous_time>= 200)){
			led_button_flag= 1;
			switch_pressed= 0;
		}
		if(led_button_flag== 1){
			PORTB^= 0xFF;
			led_button_flag= 0;
		}
    }
}

void setTimer(){
	TCCR0A= 0x02;
	TCCR0B= 0x03;
	TIMSK0= 0x02;
	OCR0A= 249;
	TCNT0= 0; 
}

void setButton(){
	DDRC= 0x00;
	PORTC= 0x01;
}

void setLed(){
	DDRB|= 0xFF;
}

void ledOn(){
	PORTB|= 0x20;
}

void ledOff(){
	PORTB&= ~0x20;
}

uint16_t getMilliseconds(){
	cli();
	uint16_t temp= milliseconds;
	sei();
	return temp;
}

