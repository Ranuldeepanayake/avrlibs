/*
 * pwm.h
 *
 * Created: 31-Jul-24 11:32:44 AM
 * Author: ranul
 *
 */ 


#ifndef PWM_H_
#define PWM_H_

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
//Channels.
#define TIMER_N_CHANNEL_A 0
#define TIMER_N_CHANNEL_B 1
//
//Output channel behavior.
#define TIMER_N_PIN_A_DISCONNECTED 0x00
#define TIMER_N_PIN_A_TOGGLE 0x40
#define TIMER_N_PIN_A_CLEAR_ON_MATCH 0x80
#define TIMER_N_PIN_A_SET_ON_MATCH 0xC0
#define TIMER_N_PIN_B_DISCONNECTED 0x00
#define TIMER_N_PIN_B_TOGGLE 0x40
#define TIMER_N_PIN_B_CLEAR_ON_MATCH 0x80
#define TIMER_N_PIN_B_SET_ON_MATCH 0xC0
//Modes.
#define TIMER_N_MODE_NORMAL 0
#define TIMER_N_MODE_PWM_PHASE_CORRECT 1
#define TIMER_N_MODE_CTC 2
#define TIMER_N_MODE_PWM_FAST 3
#define TIMER_N_MODE_PWM_PHASE_CORRECT_OCRA 4
#define TIMER_N_MODE_PWM_FAST_OCRA 5
//WGM[1:0]
#define TIMER_N_WGM_01_NORMAL 0x00
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT 0x01
#define TIMER_N_WGM_01_CTC 0x02
#define TIMER_N_WGM_01_PWM_FAST 0x03
#define TIMER_N_WGM_01_PWM_PHASE_CORRECT_OCRA 0x05
#define TIMER_N_WGM_01_PWM_FAST_OCRA 0x07
//WGM[2]
#define TIMER_N_WGM_2_NORMAL 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT 0x00
#define TIMER_N_WGM_2_CTC 0x00
#define TIMER_N_WGM_2_PWM_FAST 0x00
#define TIMER_N_WGM_2_PWM_PHASE_CORRECT_OCRA 0x80
#define TIMER_N_WGM_2_PWM_FAST_OCRA 0x80
//Interrupt switches.
#define TIMER_N_OVERFLOW_INTERRUPT_ENABLE 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_ENABLE 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_ENABLE 0x04
//Interrupt flags.
#define TIMER_N_OVERFLOW_INTERRUPT_FLAG 0x01
#define TIMER_N_OUTPUT_COMPARE_A_INTERRUPT_FLAG 0x02
#define TIMER_N_OUTPUT_COMPARE_B_INTERRUPT_FLAG 0x04


//Timer 0.
void setPWM();
void writePWM();
void setLedInterval();
void writeLedInterval();
void setIntervalPwm();
void writeIntervalPwm();
void writeInterruptPwm();
void setInterruptPwm();
void setInterruptManual();
void writeInterruptManual();
void setTimer();
void setTimerCtcToggle();

void timer0Set(uint8_t mode, uint8_t channel, uint8_t output, uint8_t prescaler);

//Timer 1.
void setTimerPwmPhaseCorrect();
void writePwmPhaseCorrect();
void setPwmFast();
void writePwmFast();
void setPwmCtc();
void setPwmInterrupt();

//Timer 2.

#endif /* PWM_H_ */