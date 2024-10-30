/*
 * MCP960X_lib.cpp
 *
 *  Created on: May 1, 2024
 *      Author: NJunker
 */

#include "MCP9601_lib.h"

void Thorlabs_MCP9601::begin(uint8_t addr)
{
	_addr = addr;

	Thorlabs_I2C_setup();
}

void Thorlabs_MCP9601::write_register(uint8_t reg, uint8_t data)
{
	uint8_t cmd[2];
	//build command word
	cmd[0] = reg;
	cmd[1] = data;
	
	Thorlabs_I2C_begin();
	Thorlabs_I2C_write(cmd, sizeof(cmd));
	Thorlabs_I2C_end();
}

uint8_t Thorlabs_MCP9601::read_register(uint8_t reg)
{
	uint8_t cmd[1];
	uint8_t _read_buf[1];
	//build command word
	cmd[0] = reg;
	
	Thorlabs_I2C_begin();
	Thorlabs_I2C_write(cmd, sizeof(cmd));
	Thorlabs_I2C_end();
	Thorlabs_I2C_read(_read_buf, sizeof(_read_buf));

	//Data is MSB first
	return _read_buf[0];
}

float Thorlabs_MCP9601::read_thermocouple()
{
	uint8_t cmd[1];
	uint8_t _read_buf[2];
	//build command word
	cmd[0] = MCP9601_HOTJUNCTION_REG;
	
	Thorlabs_I2C_begin();
	Thorlabs_I2C_write(cmd, sizeof(cmd));
	Thorlabs_I2C_end();
	
	Thorlabs_I2C_read(_read_buf, sizeof(_read_buf));

	//word is least significant byte first
	int16_t reading =  (((uint16_t) _read_buf[0]) << 8 | (uint16_t) _read_buf[1]);
	float temp = reading * 0.0625; //0.0625 degrees C per LSB in register

	return temp;
}

float Thorlabs_MCP9601::read_ambient()
{
	uint8_t cmd[1];
	uint8_t _read_buf[2];
	//build command word
	cmd[0] = MCP9601_COLDJUNCTION_REG;
	
	Thorlabs_I2C_begin();
	Thorlabs_I2C_write(cmd, sizeof(cmd));
	Thorlabs_I2C_end();
	
	Thorlabs_I2C_read(_read_buf, sizeof(_read_buf));

	//word is most significant byte first
	int16_t reading =  (((uint16_t) _read_buf[0]) << 8 | (uint16_t) _read_buf[1]);
	float temp = reading * 0.0625; //0.0625 degrees C per LSB in register

	return temp;
}

void Thorlabs_MCP9601::setConfig(thermocouple_types type, filter_coefficient coeff)
{
	//bit shift our thermocouple data to fit bits 6-4
	uint8_t data = ((uint8_t)type << 4);

	//bit-wise OR to add in our filter coefficient
	data |= (uint8_t)coeff;
	write_register(MCP9601_SENSOR_CONFIG_REG, data);
}

void Thorlabs_MCP9601::setResolution(adc_resolution res)
{
	//bit shift our thermocouple data to fit bits 6-5
	uint8_t data = ((uint8_t)res << 5);
	write_register(MCP9601_DEVICE_CONFIG_REG, data);
}

uint8_t Thorlabs_MCP9601::getStatus()
{
	return read_register(MCP9601_STATUS_REG);
}

//-----------------------------------------------------------------------
//------------------- To be implemented by user -------------------------
//-----------------------(Platform Specific)-----------------------------
//-----------------------------------------------------------------------

uint8_t Thorlabs_MCP9601::Thorlabs_I2C_write(uint8_t *buf, size_t size) {
	//Implement this in a parent class or modify for your platform

	//Take in an array of single bytes (buf) and length
	//Return length of bytes successfully transmitted
	return 0;
}

void Thorlabs_MCP9601::Thorlabs_I2C_read(uint8_t *buf, size_t size) {
	//Implement this in a parent class or modify for your platform

	//Takes in a pointer to a buffer to place data in, and size of said buffer
}

void Thorlabs_MCP9601::Thorlabs_I2C_begin() {
	//Implement this in a parent class or modify for your platform

	//Used if your platform has an I2C transaction begin function (i.e. Arduino)
}

void Thorlabs_MCP9601::Thorlabs_I2C_end() {
	//Implement this in a parent class or modify for your platform
	
	//Used if your platform has an I2C transaction end function (i.e. Arduino)
}

void Thorlabs_MCP9601::Thorlabs_I2C_setup() {
	//Implement this in a parent class or modify for your platform

	//Platform specific startup code, i.e. setting pins, clock speed, etc
}



