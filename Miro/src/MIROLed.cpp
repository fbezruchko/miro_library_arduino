#include "MIROLed.h"

#define NUM 0
#define TYPE 1

#define LED_PINS_COUNT 1

#define LED_PCOUNT 1
#define LED_VALUE 1

//using namespace miro;

const char _const_dev_name[] = "LED";

void MIROLed::Init(byte pin)
{
	this->pins[NUM] = malloc(LED_PINS_COUNT);
	this->pins[TYPE] = malloc(LED_PINS_COUNT);
	this->pins[NUM][0] = pin;
	this->pins[TYPE][0] = OUTPUT;
	pinMode(pin, OUTPUT);
}

byte MIROLed::GetPinsCount() { return LED_PINS_COUNT; };
char* MIROLed::GetName() { return _const_dev_name; }
byte MIROLed::GetParamCount() { return LED_PCOUNT; }

void MIROLed::On(byte value)
{
	analogWrite(this->pins[NUM][0], value);
}

void MIROLed::On()
{
	digitalWrite(this->pins[NUM][0], HIGH);
}

void MIROLed::Off()
{
	digitalWrite(this->pins[NUM][0], LOW);
}

void MIROLed::SetParam(byte pnum, byte *pvalue)
{
    if (pnum == LED_VALUE)
	{
		analogWrite(this->pins[NUM][0], *pvalue);
		this->value = *pvalue;
	}
}

void MIROLed::GetParam(byte pnum, byte *pvalue)
{
    if (pnum == LED_VALUE)  *pvalue = this->value;
}