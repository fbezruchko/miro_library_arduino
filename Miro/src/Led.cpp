#include "Arduino.h"

#include "Led.h"

//using namespace miro;

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "LED";

Led::Led(uint8_t* pin, uint8_t pin_count) : Device(pin, pin_count)
{
	pins[TYPE][0] = OUTPUT;
	pinMode(this->pins[NUM][0], OUTPUT);
	this->value = analogRead(this->pins[NUM][0]);
}

Led::~Led()
{
	Serial.println("LED device delete");
}

uint8_t Led::getPinsCount() { return LED_PINS_COUNT; };
char* Led::getName() { return (char*)_const_dev_name; }
uint8_t Led::getParamCount() { return LED_PCOUNT; }

void Led::setParam(uint8_t pnum, uint8_t *pvalue)
{
    if (pnum == LED_VALUE)
	{
		analogWrite(this->pins[NUM][0], *pvalue);
		this->value = *pvalue;
	}
}

void Led::getParam(uint8_t pnum, uint8_t *pvalue)
{
    if (pnum == LED_VALUE)  *pvalue = this->value;
}

Device* CreateLED(uint8_t* pin, uint8_t pin_count)
{
  Device* d = new Led(pin);
  return d;
}

void DestroyLED(Device* device)
{
	delete((Led*)device);
	return;
}