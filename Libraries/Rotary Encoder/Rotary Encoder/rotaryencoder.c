/*
 * rotaryencoder.c
 *
 * Created: 31-Jan-19 12:13:40 PM
 * Author: Ranul Deepanayake
 */ 

#include "rotaryencoder.h"

uint8_t rotary_encoder_counter= 0;

uint8_t rotary_encoder_isr_pulse_flag= 0x03;
/*
ISR pulse flag bit map.
Bit 0 (0x01): New clockwise pulse- 0.
Bit 1 (0x02): New counter clockwise pulse- 0.
Bit 2 (0x04): New clockwise pulse- 1.
Bit 3 (0x08): New counter clockwise pulse- 1.
Bit 4 (0x10): New clockwise pulse- 2.
Bit 5 (0x20): New counter clockwise pulse- 2.
Bit 6 (0x40): Reserved.
Bit 7 (0x80): Reserved.
*/

ISR(PCINT2_vect){
	//Reset counter on button press.
	if(!ROTARY_ENCODER_READ_SWITCH){
		rotary_encoder_counter= 0;
	}
	
	//Read the initial pulse (entry transition).
	if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_0) && !ROTARY_ENCODER_READ_INPUT_A && ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_isr_pulse_flag= ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_1;
		
		}else if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_0) && ROTARY_ENCODER_READ_INPUT_A && !ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_isr_pulse_flag= ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_1;
	}
	
	//Read the exit transition pulse.
	if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_1) && ROTARY_ENCODER_READ_INPUT_A && !ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_isr_pulse_flag= ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_2;
		}else if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_1) && !ROTARY_ENCODER_READ_INPUT_A && ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_isr_pulse_flag= ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_2;
	}
	
	//Read the exit pulse.
	if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_2) && ROTARY_ENCODER_READ_INPUT_A && ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_counter++;
		rotary_encoder_isr_pulse_flag= (ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_0 | ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_0);
		}else if((rotary_encoder_isr_pulse_flag & ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_2) && ROTARY_ENCODER_READ_INPUT_A && ROTARY_ENCODER_READ_INPUT_B){
		rotary_encoder_counter--;
		rotary_encoder_isr_pulse_flag= (ROTARY_ENCODER_NEW_CLOCKWISE_PULSE_0 | ROTARY_ENCODER_NEW_COUNTER_CLOCKWISE_PULSE_0);
	}
}

/*
Set up pin change interrupts to poll the rotary encoder.
*/
void rotary_encoder_set_interrupt(){
	PCICR|= ROTARY_ENCODER_PCICR_BANK;
	ROTARY_ENCODER_PCMSK_REGISTER= (ROTARY_ENCODER_INPUT_A_PIN | ROTARY_ENCODER_INPUT_B_PIN | ROTARY_ENCODER_SWITCH_PIN);
	ROTARY_ENCODER_DDR_REGISTER&= ~(ROTARY_ENCODER_INPUT_A_PIN | ROTARY_ENCODER_INPUT_B_PIN | ROTARY_ENCODER_SWITCH_PIN);
	sei();
}

/*
Set up for manually polling the rotary encoder.
*/
void rotary_encoder_set(){
	ROTARY_ENCODER_DDR_REGISTER&= ~(ROTARY_ENCODER_INPUT_A_PIN | ROTARY_ENCODER_INPUT_B_PIN | ROTARY_ENCODER_SWITCH_PIN);
}

/*
Returns the value of the counter variable.
*/
uint8_t rotary_encoder_read_interrupt(){
	uint8_t temp= 0;
	cli();
	temp= rotary_encoder_counter;
	sei();
	return temp;
}

/*
Manually polling the rotary encoder.
Access counter value using external variable 'rotary_encoder_counter';
*/
void rotary_encoder_read(){
	static uint8_t rotary_encoder_input_a_last_state= ROTARY_ENCODER_INPUT_HIGH, rotary_encoder_input_a_current_state= 0, rotary_encoder_input_b_current_state= 0;

	//Reset counter on button push.
	if(!(ROTARY_ENCODER_READ_SWITCH)){
		rotary_encoder_counter= 0;
	}
	
	if(ROTARY_ENCODER_READ_INPUT_A){
		rotary_encoder_input_a_current_state= ROTARY_ENCODER_INPUT_HIGH;
	}else{
		rotary_encoder_input_a_current_state= ROTARY_ENCODER_INPUT_LOW;
	}
	
	if(rotary_encoder_input_a_current_state!= rotary_encoder_input_a_last_state){
		if(ROTARY_ENCODER_READ_INPUT_B){
			rotary_encoder_input_b_current_state= ROTARY_ENCODER_INPUT_HIGH;
		}else{
			rotary_encoder_input_b_current_state= ROTARY_ENCODER_INPUT_LOW;
		}
		
		if(rotary_encoder_input_b_current_state!= rotary_encoder_input_a_current_state){
			rotary_encoder_counter++;
		}else{
			rotary_encoder_counter--;
		}
	}
	rotary_encoder_input_a_last_state= rotary_encoder_input_a_current_state;
}

