/*
 * inputcapture.h
 *
 * Created: 23-Nov-18 6:05:35 PM
 *  Author: ranul
 */ 


#ifndef INPUTCAPTURE_H_
#define INPUTCAPTURE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "usart328P.h"

void setCapture();
void setTestWave();
void sendCapture();

#endif /* INPUTCAPTURE_H_ */