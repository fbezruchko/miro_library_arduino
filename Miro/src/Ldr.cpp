#include "Arduino.h"

#include "Ldr.h"

#define LDR_PINS_COUNT 1
#define LDR_PCOUNT 1
#define LDR_VALUE 1

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "LDR";
const uint8_t _default_pins[] = {16};
const char *const _pin_names[LDR_PINS_COUNT] = {"VAL"};

Ldr::Ldr(uint8_t *pins, uint8_t pins_count = LDR_PINS_COUNT) : Device(pins, pins_count)
{
	this->_pins[TYPE][0] = OUTPUT;
	pinMode(this->_pins[NUM][0], OUTPUT);
	this->_value = analogRead(this->_pins[NUM][0]);
}

Ldr::~Ldr()
{
	//Serial.println("LDR device delete");
}

uint8_t Ldr::getPinsCount() { return LDR_PINS_COUNT; };
//char *Ldr::getName() { return (char *)_const_dev_name; }
uint8_t Ldr::getParamCount() { return LDR_PCOUNT; }

void Ldr::setParam(uint8_t pnum, uint8_t *pvalue)
{
	return;
}

uint8_t *Ldr::getParam(uint8_t pnum)
{
	if (pnum == 0)
	{
		return (uint8_t *)_const_dev_name;
	}
	if (pnum == LDR_VALUE)
	{
		this->_value = analogRead(this->_pins[NUM][0]);
		return (uint8_t *)(&this->_value);
	}
	else
	{
		return nullptr;
	}
}

uint8_t Ldr::getParamType(uint8_t pnum)
{
	if (pnum == 0)
		return PTYPE_CSTRING;
	if (pnum == LDR_VALUE)
		return PTYPE_UINT16;
	return 0;
}

Device *createLDR(uint8_t *pins)
{
	Device *d = new Ldr(pins);
	return d;
}

Device *createLDR()
{
	Device *d = new Ldr((uint8_t *)_default_pins);
	return d;
}

void destroyLDR(Device *device)
{
	delete ((Ldr *)device);
	return;
}

uint8_t getDefaultPinLDR(uint8_t pinIndex)
{
	return _default_pins[pinIndex];
}

const char *getPinNameLDR(uint8_t pinIndex)
{
	return _pin_names[pinIndex];
}