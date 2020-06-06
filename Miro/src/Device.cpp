#include "Device.h"
#include "Arduino.h"

//using namespace miro;

#define NUM 0
#define TYPE 1

Device::~Device()
{
	if (this->_pins[0])
		free(this->_pins[0]);
	if (this->_pins[1])
		free(this->_pins[1]);
	//Serial.println("Base device delete");
}

Device::Device(uint8_t *pins, uint8_t pins_count)
{
	this->_pins[NUM] = (uint8_t *)malloc(pins_count);
	this->_pins[TYPE] = (uint8_t *)malloc(pins_count);
	for (uint8_t i = 0; i < pins_count; i++)
	{
		this->_pins[NUM][i] = pins[i];
	}
	return;
}

void Device::sync()
{
	return;
}

void Device::setParam(uint8_t pnum, uint8_t *pvalue)
{
	return;
}

uint8_t *Device::getParam(uint8_t pnum, uint8_t *pvalue)
{
	return nullptr;
}

uint8_t Device::getPinsCount()
{
	return 0;
}

char *Device::getName()
{
	return 0;
}

uint8_t Device::getParamCount()
{
	return 0;
}

uint8_t Device::getPinNum(uint8_t pin_num)
{
	return this->_pins[NUM][pin_num];
}

uint8_t Device::getPinType(uint8_t pin_num)
{
	return this->_pins[TYPE][pin_num];
}