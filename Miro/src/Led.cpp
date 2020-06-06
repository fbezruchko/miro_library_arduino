#include "Arduino.h"

#include "Led.h"

//using namespace miro;

#define LED_PINS_COUNT 1
#define LED_PCOUNT 1
#define LED_VALUE 1

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "LED";
const uint8_t default_pins[] = {13};

Led::Led(uint8_t *pins, uint8_t pins_count = LED_PINS_COUNT) : Device(pins, pins_count)
{
	this->_pins[TYPE][0] = OUTPUT;
	pinMode(this->_pins[NUM][0], OUTPUT);
	this->_value = analogRead(this->_pins[NUM][0]);
}

Led::~Led()
{
	//Serial.println("LED device delete");
}

uint8_t Led::getPinsCount() { return LED_PINS_COUNT; };
char *Led::getName() { return (char *)_const_dev_name; }
uint8_t Led::getParamCount() { return LED_PCOUNT; }

void Led::setParam(uint8_t pnum, uint8_t *pvalue)
{
	if (pnum == LED_VALUE)
	{
		analogWrite(this->_pins[NUM][0], *pvalue);
		this->_value = *pvalue;
	}
}

uint8_t *Led::getParam(uint8_t pnum, uint8_t *pvalue)
{
	if (pnum == LED_VALUE)
	{
		*pvalue = this->_value;
		return pvalue;
	}
	else
	{
		return nullptr;
	}
}

Device *createLED(uint8_t *pins)
{
	Device *d = new Led(pins);
	return d;
}

Device *createLED()
{
	Device *d = new Led((uint8_t *)default_pins);
	return d;
}

void destroyLED(Device *device)
{
	delete ((Led *)device);
	return;
}