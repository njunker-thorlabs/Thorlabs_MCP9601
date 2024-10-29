/******************************************************************************
This I2C driver is used to interface the Microchip MCP9601 Thermocouple chip.

   \date          1-May-2024
   \copyright     2024 Thorlabs Spectral Works
   \author       Nicholas Junker (njunker@thorlabs.com)

******************************************************************************/

#ifndef INC_MCP9601_LIB_H_
#define INC_MCP9601_LIB_H_

//#include <Arduino.h>
//#include <Wire.h>
#include <cstdint> //for uint8_t, etc
#include <cstddef> //for size_t

//Default I2C instance used
//extern I2C_HandleTypeDef hi2c1;

#define MCP9601_HOTJUNCTION_REG 0x00
#define MCP9601_JUNCTIONDELTA_REG 0x01
#define MCP9601_COLDJUNCTION_REG 0x02
#define MCP9601_RAWDATA_REG 0x03
#define MCP9601_STATUS_REG 0x04
#define MCP9601_SENSOR_CONFIG_REG 0x05
#define MCP9601_DEVICE_CONFIG_REG 0x06
#define MCP9601_DEVICEID_REG 0x20

class Thorlabs_MCP9601 {
public:
	//TODO make some nicer functions
	typedef enum {
	  TYPE_K,
	  TYPE_J,
	  TYPE_T,
	  TYPE_N,
	  TYPE_S,
	  TYPE_E,
	  TYPE_B,
	  TYPE_R
	} thermocouple_types;

	typedef enum {
	  FILTER_OFF,
	  FILTER_1, //minimum filter
	  FILTER_2,
	  FILTER_3,
	  FILTER_4, //mid filter
	  FILTER_5,
	  FILTER_6,
	  FILTER_7 //max filter
	} filter_coefficient;

	typedef enum {
	  RES_18_BIT, //320ms conversion
	  RES_16_BIT, //80ms conversion
	  RES_14_BIT, //20ms conversion
	  RES_12_BIT //5ms conversion
	} adc_resolution;


	//Initialize MCP with I2C address.
	void begin(uint8_t addr = 0x65);

	//Read thermocouple temperature, calculate degrees C
	float read_thermocouple();

	//Read ambient temperature, calculate degrees C
	float read_ambient();

	//Set thermocouple type and filter coefficent
	void setConfig(thermocouple_types type, filter_coefficient coeff);

	//Set internal ADC resolution
	void setResolution(adc_resolution res);

	//Get status register. Short/open circuit detection, alerts 1-4
	uint8_t getStatus();

protected:

	uint8_t _addr;

	virtual uint8_t Thorlabs_I2C_write(uint8_t *buf, size_t size);

	virtual void Thorlabs_I2C_read(uint8_t *buf, size_t size);

	virtual void Thorlabs_I2C_begin();

	virtual void Thorlabs_I2C_end();

	virtual void Thorlabs_I2C_setup();

private:

	void write_register(uint8_t reg, uint8_t data);
	uint8_t read_register(uint8_t reg);
};



#endif /* INC_MCP9601_LIB_H_ */
