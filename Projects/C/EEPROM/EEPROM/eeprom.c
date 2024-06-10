/*
 * eeprom.c
 *
 * Created: 10-Nov-18 4:01:44 PM
 *  Author: Ranul Deepanayake
 */ 
#include "eeprom.h"

uint8_t eepromWrite(uint8_t data){
	uint8_t status= 100;
	status= i2cDelayedStart(0x50, I2C_WRITE);
	status= i2cWrite(0x00);	//Address high byte.
	status= i2cWrite(0x01);	//Address low byte.
	status= i2cWrite(data);	//Address low byte.
	stop();
	return status;
}

uint8_t eepromWritePage(uint8_t *data, uint8_t number_of_bytes, uint8_t page_size, uint16_t page_number){
	uint16_t page_address= 0; 
	uint8_t address_high_byte= 0, address_low_byte= 0;
	//Add provisions for busy slave device from the calling function.
	//Code 0x50 in i2c.c.
	if(number_of_bytes- 1> page_size- 1){
		return 50;
	}
	
	page_address= (page_number* page_size)- page_size;
	address_low_byte= page_address;
	address_high_byte= page_address>> 8; 
	
	uint8_t status= SUCCESS;
	if(i2cDelayedStart(0x50, I2C_WRITE)== SLAVE_ADDRESS_UNACKNOWLEDGED){ 
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	if(i2cWrite(address_high_byte)== SLAVE_DATA_UNACKNOWLEDGED){	//Address high byte.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	if(i2cWrite(address_low_byte)== SLAVE_DATA_UNACKNOWLEDGED){		//Address high byte.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	
	uint8_t incrementer= 0, retry_count= 0;
	
	while(incrementer< number_of_bytes){
		if(retry_count== 100){	//Retry this many times.
			status= 50;	//Return error code.
			break;	//Exit the loop.
		}
		if(i2cWrite(*data)== SLAVE_DATA_UNACKNOWLEDGED){	//Send data byte and check for acknowledgement. //*(data+ incrementer)
			retry_count++;
			continue;	//Skip to the next iteration without incrementing the incrementer.
		}
		incrementer++;
		retry_count= 0;	//Reset the retry count if the slave finally accepts data.
		PORTB^= 0x20;
	}
	stop();
	
	PORTB|= 0x20;
	_delay_ms(10); //Wait for the EEPROM to write to the cells (5ms stated in the datasheet).
	PORTB&= ~0x20;
	return status;
}

void eepromRead(){
	
}

uint8_t eepromReadPage(uint8_t *data, uint8_t page_size, uint16_t page_number){
	uint16_t page_address= 0;
	uint8_t address_high_byte= 0, address_low_byte= 0;
	char data_string[5];
	
	page_address= (page_number* page_size)- page_size;
	address_low_byte= page_address;
	address_high_byte= page_address>> 8;
	
	uint8_t status= SUCCESS;
	if(i2cDelayedStart(0x50, I2C_WRITE)== SLAVE_ADDRESS_UNACKNOWLEDGED){
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	if(i2cWrite(address_high_byte)== SLAVE_DATA_UNACKNOWLEDGED){	//Address high byte.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	if(i2cWrite(address_low_byte)== SLAVE_DATA_UNACKNOWLEDGED){		//Address high byte.
		return SLAVE_DATA_UNACKNOWLEDGED;
	}
	if(i2cDelayedStart(0x50, I2C_READ)== SLAVE_ADDRESS_UNACKNOWLEDGED){
		return SLAVE_ADDRESS_UNACKNOWLEDGED;
	}
	
	uint8_t incrementer= 0, reception_error_count= 0, data_byte= 0;
	
	while(incrementer< page_size){
		if(reception_error_count== 100){	//Retry this many times.
			status= 50;	//Return error code.
			break;	//Exit the loop.
		}
		
		if(incrementer== page_size- 1){	//Check for the last byte of the page.
			data_byte= i2cReadLastByte();
			sprintf(data_string, "%x", data_byte);
			println(data_string);
			break;	//Exit after the last byte of the page has been read.
		}
		
		data_byte= i2cReadByte();
		if(data_byte== MASTER_DATA_UNACKNOWLEDGED){
			reception_error_count++;
			continue;
			//data_byte= 0; //Provide a default value.
			//How to decrement the EEPROM's internal address register?
			//Return as a temporary solution?
		}
		sprintf(data_string, "%x", data_byte);
		println(data_string);
		PORTB^= 0x20;
		
		incrementer++;
		reception_error_count= 0;	//Reset the retry count if the slave finally accepts data.
	}
	stop();
	return status;
}
