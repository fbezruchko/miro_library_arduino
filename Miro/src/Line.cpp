#include "Arduino.h"

#include "Line.h"

#define LINE_PINS_COUNT 1
#define LINE_PCOUNT 1
#define LINE_VALUE 1

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "LINE";
const uint8_t _default_pins[] = {15};
const char *const _pin_names[LINE_PINS_COUNT] = {"VAL"};

Line::Line(uint8_t *pins, uint8_t pins_count = LINE_PINS_COUNT) : Device(pins, pins_count)
{
	this->_pins[TYPE][0] = OUTPUT;
	pinMode(this->_pins[NUM][0], OUTPUT);
	this->_value = analogRead(this->_pins[NUM][0]);
}

Line::~Line()
{
	//Serial.println("LINE device delete");
}

uint8_t Line::getPinsCount() { return LINE_PINS_COUNT; };
//char *Line::getName() { return (char *)_const_dev_name; }
uint8_t Line::getParamCount() { return LINE_PCOUNT; }

void Line::setParam(uint8_t pnum, uint8_t *pvalue)
{
	return;
}

uint8_t *Line::getParam(uint8_t pnum)
{
	if (pnum == 0)
	{
		return (uint8_t *)_const_dev_name;
	}
	if (pnum == LINE_VALUE)
	{
		this->_value = analogRead(this->_pins[NUM][0]);
		return (uint8_t *)(&this->_value);
	}
	else
	{
		return nullptr;
	}
}

uint8_t Line::getParamType(uint8_t pnum)
{
	if (pnum == 0)
		return PTYPE_CSTRING;
	if (pnum == LINE_VALUE)
		return PTYPE_UINT16;
	return 0;
}

Device *createLINE(uint8_t *pins)
{
	Device *d = new Line(pins);
	return d;
}

Device *createLINE()
{
	Device *d = new Line((uint8_t *)_default_pins);
	return d;
}

void destroyLINE(Device *device)
{
	delete ((Line *)device);
	return;
}

uint8_t getDefaultPinLINE(uint8_t pinIndex)
{
	return _default_pins[pinIndex];
}

const char *getPinNameLINE(uint8_t pinIndex)
{
	return _pin_names[pinIndex];
}