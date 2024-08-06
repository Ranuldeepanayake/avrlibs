#include "main.h"

enum button_press{DEPRESSED, PRESSED};

uint16_t elapsed_time= 0;
uint16_t previous_time= 0;
uint16_t total_time= 5000;
uint8_t button_state= DEPRESSED;
uint8_t button_press_count= 0;

int main(){
	setTimer();
	setPins();
	
	while(1){
		elapsed_time= millis();
		if(elapsed_time- previous_time== total_time){
			blinkLed(button_press_count);
			button_press_count= 0;
			previous_time= millis();
			}else{
			countPresses(readPortB());
		}
	}
}

void setPins(){
	DDRB|= 0x20;	//Set input ports and onboard LED.
	PORTB|= 0x01;	//Set pull-up resistors.
}

uint8_t readPortB(){
	return PINB;	//Return input port status.
}

void countPresses(int port_b_result){
	if(!(port_b_result & 0x01)){
		if(button_state== PRESSED){
			
			}else{
			button_state= PRESSED;
		}
	}
	else{
		if(button_state== PRESSED){
			button_state= DEPRESSED;
			button_press_count++;
			_delay_ms(10); //For debouncing.
		}
	}
}

void blinkLed(int presses){
	int i;
	for(i= 0; i< presses; i++){
		PORTB|= 0x20;
		_delay_ms(100);
		PORTB&= ~0x20;
		_delay_ms(100);
		//PORTB ^= 0x20;
	}
}
