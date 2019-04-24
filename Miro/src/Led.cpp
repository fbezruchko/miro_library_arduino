#include "Led.h"

#define NUM 0
#define TYPE 1

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
}

void Led::On()
{
	digitalWrite(this->pins[0][NUM], HIGH);
}

void Led::Off()
{
	digitalWrite(this->pins[0][NUM], LOW);
}

void Led::SetParam(byte pnum, float pvalue)
{
    if (pnum == LED_VALUE) analogWrite(this->pins[0][NUM], pvalue);
}

void Led::GetParam(byte pnum, byte *pvalue)
{
    if (pnum == LED_VALUE)  *pvalue = this->led_value;
}

