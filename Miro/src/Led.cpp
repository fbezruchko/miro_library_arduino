#include "Led.h"

#define NUM 0
#define TYPE 1

#define PNUM_VALUE 0

using namespace miro;

const char _const_dev_name[] = "LED";

void Led::Init(byte pin)
{
	this->pins_count = 1;
	this->pins[0][NUM] = pin;
	this->pins[0][TYPE] = OUTPUT;
	pinMode(pin, OUTPUT);
    this->_dev_name = _const_dev_name;
    this->_param_count = 1;
}

void Led::On(byte value)
{
	analogWrite(this->pins[0][NUM], value);
	this->led_value = value;
}

void Led::On()
{
	digitalWrite(this->pins[0][NUM], HIGH);
	this->led_value = 255;
}

void Led::Off()
{
	digitalWrite(this->pins[0][NUM], LOW);
	this->led_value = 0;
}

void Led::SetParam(byte pnum, float pvalue)
{
    if (pnum == PNUM_VALUE) {
		analogWrite(this->pins[0][NUM], pvalue);
		this->led_value = pvalue;
	}
}

void Led::GetParam(byte pnum, byte *pvalue)
{
    if (pnum == PNUM_VALUE)  *pvalue = this->led_value;
}