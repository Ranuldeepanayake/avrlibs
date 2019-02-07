/*
 * analogwrite.c
 *
 * Created: 05-Feb-19 9:03:11 PM
 * Author: Ranul Deepanayake
 */ 

#include "analogwrite.h"

/*
Set up PWM on the specified port and OCR pin.
Selectable fixed frequencies.
Selectable waveform and output compare modes.
One timer module can operate only at one frequency for both output compare units.
Phase correct PWM operates at half the frequency of fast PWM (defined frequencies are for fast PWM).
Timers can't be used if being used for another purpose.
*/
void analog_write_pwm_set(uint8_t timer, uint8_t frequency, uint8_t waveform, uint8_t ocr_mode){
	//Setting the prescaler bits according to the clock speeds and specified frequency.
	if(F_CPU_VALUE== F_CPU_16_MHZ){
		if(timer== TIMER_0){
			if(frequency== TIMER8_16_CLCK_62_KHZ){
				TIMER_0_PRESCALER_SELECT_REGISTER|= TIMER8_16_CLCK_62_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_16_CLCK_7_KHZ){
				TIMER_0_PRESCALER_SELECT_REGISTER|= TIMER8_16_CLCK_7_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_16_CLCK_976_HZ){
				TIMER_0_PRESCALER_SELECT_REGISTER|= TIMER8_16_CLCK_976_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_16_CLCK_244_HZ){
				TIMER_0_PRESCALER_SELECT_REGISTER|= TIMER8_16_CLCK_244_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_16_CLCK_61_HZ){
				TIMER_0_PRESCALER_SELECT_REGISTER|= TIMER8_16_CLCK_61_HZ_PRESCALER_BITS;
			}
		}
		else if(timer== TIMER_1){
			if(frequency== TIMER16_16_CLCK_16_bit_15_KHZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_16_bit_15_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_16_bit_2_KHZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_16_bit_2_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_16_bit_244_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_16_bit_244_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_16_bit_61_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_16_bit_61_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_16_bit_15_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_16_bit_15_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_8_bit_62_KHZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_8_bit_62_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_8_bit_7_KHZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_8_bit_7_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_8_bit_976_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_8_bit_976_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_8_bit_244_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_8_bit_244_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER16_16_CLCK_8_bit_61_HZ){
				TIMER_1_PRESCALER_SELECT_REGISTER|= TIMER16_16_CLCK_8_bit_61_HZ_PRESCALER_BITS;
			}
		}
		else if(timer== TIMER_2){
			if(frequency== TIMER8_2_16_CLCK_62_KHZ){
				TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_62_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_7_KHZ){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_7_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_2_KHZ_PRESCALER_BITS){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_2_KHZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_976_HZ){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_976_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_488_HZ){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_488_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_244_HZ){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_244_HZ_PRESCALER_BITS;
			}else if(frequency== TIMER8_2_16_CLCK_61_HZ){
					TIMER_2_PRESCALER_SELECT_REGISTER|= TIMER8_2_16_CLCK_61_HZ_PRESCALER_BITS;
			}
		}
		
		}else if(F_CPU_VALUE== F_CPU_8_MHZ){
		
		}else if(F_CPU_VALUE== F_CPU_1_MHZ){
		
		}
	
	//Select waveform generation.
	if(timer== TIMER_0){
		TCCR0A|= waveform;
	}
	else if(timer== TIMER_1){
		if(waveform== TIMER16_WAVEFORM_GENERATION_NORMAL){
			TIMER16_WAVEFORM_GENERATION_NORMAL_SELECTOR
		}else if(waveform== TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_16_BIT){
			TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_16_BIT_SELECTOR
		}else if(waveform== TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_8_BIT){
			TIMER16_WAVEFORM_GENERATION_PHASE_CORRECT_8_BIT_SELECTOR
		}else if(waveform== TIMER16_WAVEFORM_GENERATION_FAST_16_BIT){
			TIMER16_WAVEFORM_GENERATION_FAST_16_BIT_SELECTOR
		}else if(waveform== TIMER16_WAVEFORM_GENERATION_FAST_8_BIT){
			TIMER16_WAVEFORM_GENERATION_FAST_8_BIT_SELECTOR
		}
	}else if(timer== TIMER_2){
		TCCR2A|= waveform;
	}
	
	//Set output compare mode.
	if(timer== TIMER_0){
		TCCR0A|= ocr_mode; 
	}else if(timer== TIMER_1){
		TCCR1A|= ocr_mode;	
	}else if(timer== TIMER_2){
		TCCR2A|= ocr_mode;
	}
}

/*
Write PWM value to output compare pin.
DDR pin must be set to output.
For timer 0 and 2, OCR values range from 0- 255.
For timer 1 running in 8 bit mode, OCR values range from 0- 255.
For timer 1 running in 10 bit mode, OCR values range from 0- 1023.
*/
void analog_write_pwm_write(uint8_t timer, uint8_t ocr, uint16_t duty_cycle){
	switch(timer){
		case TIMER_0:
			switch(ocr){
				case OCR_A:
					OCR0A= (uint8_t) duty_cycle;
					break;
				case OCR_B:
					OCR0B= (uint8_t) duty_cycle;
					break;
				default:
					OCR0A= (uint8_t) duty_cycle;
					break;
			}	
			break;
			
		case TIMER_1:
			switch(ocr){
				case OCR_A:
					OCR1AH= (duty_cycle>> 8);
					OCR1AL= (uint8_t) duty_cycle;
					break;
				case OCR_B:
					OCR1BH= (duty_cycle>> 8);
					OCR1BL= (uint8_t) duty_cycle;
					break;
				default:
					OCR1AH= (duty_cycle>> 8);
					OCR1AL= (uint8_t) duty_cycle;
				break;
			}
			break;
			
		case TIMER_2:
			switch(ocr){
				case OCR_A:
					OCR2A= (uint8_t) duty_cycle;
					break;
				case OCR_B:
					OCR2B= (uint8_t) duty_cycle;
					break;
				default:
					OCR2A= (uint8_t) duty_cycle;
					break;
			}
			break;	
	}	
}

/*
Set up frequency generator.
CTC, fast PWM and phase and frequency correct PWM modes are supported.
Prescaler is set to 1. 
Can't run while timer 1 is being used for another purpose.
Duty cycle generation varies with the TOP value. Therefore experimentation is necessary to get the duty cycle range for a certain frequency (I couldn't figure out
the equation).
*/
void analog_write_frequency_set(uint8_t waveform){
	if(waveform== TIMER16_FREQUENCY_WAVEFORM_GENERATION_CTC){
		//Mode 12.
		TCCR1A= 0x40;
		TCCR1B= 0x19;
		TCNT1= 0x0000;
		OCR1A= 0x0000;
	}else if(waveform== TIMER16_FREQUENCY_WAVEFORM_GENERATION_FAST){
		//Mode 14.
		TCCR1A= 0x82;
		TCCR1B= 0x19;
		TCNT1= 0x0000;
		OCR1A= 0x0000;
	}else if(waveform== TIMER16_FREQUENCY_WAVEFORM_GENERATION_PHASE_AND_FREQUENCY_CORRECT){
		//Mode 8.
		TCCR1A= 0x80;
		TCCR1B= 0x11;
		TCNT1= 0x0000;
		OCR1A= 0x0000;
	}
}

/*
Generate the specified frequency and duty cycle.
Duty cycle is undefined by default.
CTC mode has a fixed duty cycle of 50%.
ICR1 is used as the top (frequency selector) and OCR1 as the PWM selector.
*/
void analog_write_frequency_write(uint8_t waveform, uint32_t frequency, uint16_t duty_cycle){
	switch(waveform){
		case TIMER16_FREQUENCY_WAVEFORM_GENERATION_CTC:
			ICR1= (16000000UL/(frequency* 2UL* 1UL))- 1UL;
			break;
		case TIMER16_FREQUENCY_WAVEFORM_GENERATION_FAST:
			ICR1= (16000000UL/ (1UL* frequency))- 1UL;
			OCR1A= duty_cycle;
			break;
		case TIMER16_FREQUENCY_WAVEFORM_GENERATION_PHASE_AND_FREQUENCY_CORRECT:
			ICR1= 16000000UL/ (2UL* 1UL* frequency);	
			OCR1A= duty_cycle;
			break;
	}
}

