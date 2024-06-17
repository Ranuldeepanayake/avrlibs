/*
 * ECU Simulator.cpp
 *
 * Created: 11-Jun-24 11:21:06 AM
 * Author : ranul
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
extern "C" {
	#include "i2c.h"
}
#include "main.h"


//using namespace std;

//Virtual register memory. Eight bit virtual registers have been used.
uint8_t register_memory [REGISTER_MEMORY_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
	
void setLed(){
	DDRB|= 0x20;
}

void blinkLed(){
	PORTB^= 0x20;
}

int main(void)
{
	setLed();
    i2cSetSlave(I2C_PRESCALER, I2C_BAUD_RATE, I2C_SLAVE_ADDRESS);
	
    while (1) 
    {
		if (i2cSlaveReceivedNewWriteData()){
			commandDecoder(i2cSlaveReadAck());
			blinkLed();
		}
    }
}

void commandDecoder(uint8_t command){

	//For read commands.
	if(command > COMMAND_MAP_READ_RANGE_LOW && command < COMMAND_MAP_READ_RANGE_HIGH){
		switch (command)
		{
			case COMMAND_READ_BRAKE_SWITCH_STATE:
				//sendData(readRegister(REGISTER_READ_BRAKE_SWITCH_STATE));
			case COMMAND_READ_CONTROLLER_STATUS_CODE:
				//sendData(readRegister(REGISTER_READ_CONTROLLER_STATUS_CODE));
			case COMMAND_READ_CONTROLLER_VOLTAGE:
				//sendData(readRegister(REGISTER_READ_CONTROLLER_VOLTAGE_LOW)); sendData(readRegister(REGISTER_READ_CONTROLLER_VOLTAGE_HIGH));
			case COMMAND_READ_FAULT_CODE:
				//sendData(readRegister(REGISTER_READ_FAULT_CODE));
			case COMMAND_READ_INPUT_VOLTAGE:
				//sendData(readRegister(REGISTER_READ_INPUT_VOLTAGE_LOW)); sendData(readRegister(REGISTER_READ_INPUT_VOLTAGE_HIGH));
			case COMMAND_READ_SOLENOID_FR_STATE: //***Test command.
			//readRegister(REGISTER_READ_SOLENOID_FR_STATE);
				sendData(readRegister(REGISTER_READ_SOLENOID_FR_STATE), TWO_BYTES);
			//case default:
				//break;
		}
	//For write commands.
	}else if (command > COMMAND_MAP_WRITE_RANGE_LOW && command < COMMAND_MAP_WRITE_RANGE_HIGH){
		
	}
	
}

//void sendData(uint8_t *data, uint8_t length){
	//for(uint8_t incrementer = 0; incrementer < length; incrementer++){
		//i2cSlaveSendData(*data + incrementer);
		//while(!i2cSlaveSentNewReadData());
		//i2cSlaveSetMoreDataToSend();
	//}
//}
//
//uint8_t *readRegister(uint8_t address){
	//return &register_memory[address];
//}

void sendData(uint8_t data, uint8_t length){
	for(uint8_t incrementer = 0; incrementer < length; incrementer++){
		i2cSlaveSendData(0xFF);
		while(!i2cSlaveSentNewReadData());
		i2cSlaveSetMoreDataToSend();
	}
}

uint8_t readRegister(uint8_t address){
	return 0xFF;
}

/*
1. MCU carries out internal functions. 
2. MCU listens on the i2c bus.
3. MCU receives command and decodes the appropriate internal function.
4. MCU sends back a response.  
*/

const char *descriptors[13][2] = {
	{"class", "brake"},
	{"sub-class", "abs"},
	{"make", "bosch"},
	{"model", "abs-5.3"},
	{"serial", "01AB2476C011"},
	{"batch", "H3F-057"},
	{"dom", "2024-06-11"},
	{"com", "germany"},
	{"bus", "k-line"},
	{"channels", "4"},
	{"solenoids", "8"},
	{"capability-1", "abs"},
	{"capability-2", "traction-control"}
};

//const char *names[2][2] PROGMEM = {{"Gehan", "beepboop"}, {"Gehan", "beepboop"}};
