#include "Device.h"
#include "Arduino.h"

//using namespace miro;

#define NUM 0
#define TYPE 1

Device::~Device()
{
	if (this->pins[0]) free(this->pins[0]);
	if (this->pins[1]) free(this->pins[1]);
	Serial.println("Base device delete");
}

Device::Device(uint8_t* pin, uint8_t pin_count)
{
	this->pins[NUM] = (uint8_t*)malloc(pin_count);
	this->pins[TYPE] = (uint8_t*)malloc(pin_count);
	for (uint8_t i = 0; i < pin_count; i++)
	{
		this->pins[NUM][i] = pin[i];
	}
	return;
}

void Device::Sync()
{
	return;
}

void Device::setParam(uint8_t pnum, uint8_t *pvalue)
{
	return;
}

void Device::getParam(uint8_t pnum, uint8_t *pvalue)
{
	return;
}

uint8_t Device::getPinsCount()
{
	return 0;
}

char* Device::getName()
{
	return 0;
}

uint8_t Device::getParamCount()
{
	return 0;
}