/*
 * PWM.c
 *
 * Created: 04-Oct-18 5:26:56 PM
 * Author : Ranul Deeepanayake
 * Demonstrates the use of PWM using timers and counters.
 * Timer modes of operation: Fast PWM- good for communication, phase correct PWM- good for motor and light control, frequency and phase correct PWM.
 * Compare output modes: Disabled, normal, toggle, clear on match (non- inverting) and set on match (inverting). 
 * Timer sets the frequency.
 * Counter sets PWM.
 * ***Fast PWM has twice the frequency of phase correct PWM.	
 */ 

#ifndef PWM_H_
#define PWM_H_
//Timers.
#define TIMER_0 0
#define TIMER_1 1
#define TIMER_2 2
//Channels.
#define TIMER_N_CHANNEL_A 0
#define TIMER_N_CHANNEL_B 1
//Prescalers.
#define TIMER_N_PRESCALER_NONE 0x00
#define TIMER_N_PRESCALER_1 0x01
#define TIMER_N_PRESCALER_8 0x02
#define TIMER_N_PRESCALER_64 0x03
#define TIMER_N_PRESCALER_256 0x04
#define TIMER_N_PRESCALER_1024 0x05
#define TIMER_N_PRESCALER_EXTERNAL_FALLING 0x06
#define TIMER_N_PRESCALER_EXTERNAL_RISING 0x07
//Force compare switches.
#define TIMER_N_FORCE_COMPARE_A 0x80
#define TIMER_N_FORCE_COMPARE_B 0x40
//Output channel behavior.
#define TIMER_N_PIN_A_DISCONNECTED 0x00
#define TIMER_N_PIN_A_TOGGLE 0x40
#define TIMER_N_PIN_A_CLEAR_ON_MATCH 0x80
#define TIMER_N_PIN_A_SET_ON_MATCH 0xC0
#define TIMER_N_PIN_B_DISCONNECTED 0x00
#define TIMER_N_PIN_B_TOGGLE 0x10
#define TIMER_N_PIN_B_CLEAR_ON_MATCH 0x20
#define TIMER_N_PIN_B_SET_ON_MATCH 0x30
//Modes.
#define TIMER_N_MODE_NORMAL 0
#define TIMER_N_MODE_PWM_PHASE_CORRECT 1
#define TIMER_N_MODE_CTC 2
#define TIMER_N_MODE_PWM_FAST 3
#define TIMER_N_MODE_PWM_PHASE_CORRECT_OCR 4
#define TIMER_N_MODE_PWM_FAST_OCR 5
//WGM[1:0]
#define TIMER_N_WGM_01_NORMAL 0x00
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT 0x01
#define TIMER_N_WGM_01_CTC 0x02
#define TIMER_N_WGM_01_PWM_FAST 0x03
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCR 0x05
#define TIMER_N_WGM_01_PWM_FAST_OCR 0x07
//WGM[2]
#define TIMER_N_WGM_2_NORMAL 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT 0x00
#define TIMER_N_WGM_2_CTC 0x00
#define TIMER_N_WGM_2_PWM_FAST 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCR 0x08
#define TIMER_N_WGM_2_PWM_FAST_OCR 0x08
//Interrupt switches.
#define TIMER_N_INTERRUPT_NONE 0x00
#define TIMER_N_OVERFLOW_INTERRUPT_ENABLE 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_ENABLE 0x04
//Interrupt flags.
#define TIMER_N_OVERFLOW_INTERRUPT_FLAG 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_FLAG 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_FLAG 0x04
//Output DD register and pin mapping.
#ifndef MCU 
	#define MCU atmega328p
#endif
#if MCU == atmega328p
	//DDR register.
	#define TIMER_0_PIN_A_DDR_REGISTER 0x0A
	#define TIMER_0_PIN_B_DDR_REGISTER 0x0A
	#define TIMER_1_PIN_A_DDR_REGISTER 0x04
	#define TIMER_1_PIN_B_DDR_REGISTER 0x04
	#define TIMER_2_PIN_A_DDR_REGISTER 0x04
	#define TIMER_2_PIN_B_DDR_REGISTER 0x0A
	//Pin number.
	#define TIMER_0_PIN_A (1 << PORTD6) 
	#define TIMER_0_PIN_B (1 << PORTD5) 
	#define TIMER_1_PIN_A (1 << PORTB1)
	#define TIMER_1_PIN_B (1 << PORTB2)
	#define TIMER_2_PIN_A (1 << PORTB3)
	#define TIMER_2_PIN_B (1 << PORTD3)
#endif
//Millis and micros.
#define TIMER_MILLIS_OCR 0xF9 //249
#define TIMER_MICROS_OCR 0x01 //1
#define TIMER_MICROS_MULTIPLIER 8

/*
Functions.
*/
//Timer 0.
void timer0Set(uint8_t mode, uint8_t output, uint8_t prescaler);
void timer0Write(uint8_t channel, uint8_t value);
//Timer 1.


//Timer 2.
void timer2Set(uint8_t mode, uint8_t output, uint8_t prescaler);
void timer2Write(uint8_t channel, uint8_t value);
//Set timer to count milliseconds.
void timer2SetMillis();
//Set timer to count microseconds.
void timer2SetMicros();
//Get milliseconds.
uint16_t timer2GetMillis();
//Get microseconds.
uint16_t timer2GetMicros();
//Common.
void timerSetOutputPins(uint8_t timer, uint8_t channel);
void timerSetPrescaler(uint8_t timer, uint8_t prescaler);
void timerEnableInterrupts(uint8_t timer, uint8_t interrupt);
void timerDisableInterrupts(uint8_t timer, uint8_t interrupt);

/*

Example implementation for PWM or frequency generation.

int main(void)
{
	adcSet(ADC_MODE_SINGLE_CONVERSION, ADC_PRESCALER_128, ADC_VREF_AVCC);
	timerSetOutputPins(TIMER_2, TIMER_N_CHANNEL_B);
	timer2Set(TIMER_N_MODE_PWM_FAST, TIMER_N_PIN_B_CLEAR_ON_MATCH, TIMER_N_PRESCALER_256);
	
	while (1)
	{
		timer2Write(TIMER_N_CHANNEL_B, (uint8_t) (adcRead(ADC_0) / 4));
	}
}


Example implementation for millis and micros.

int main(void)
{
	uint16_t previous_time= 0, current_time= 0, on_time= 500, total_time= 1000;
	timer2SetMillis();
	DDRB |= 0x20;
	
	while (1)
	{
		current_time= timer2GetMillis();
		if((current_time- previous_time) >= total_time){
			PORTB |= 0x20;
			previous_time = timer2GetMillis();
		}
		
		if(current_time - previous_time >= on_time){
			PORTB &= ~(0x20);
		}
	}
}

*/

#endif /* PWM_H_ */