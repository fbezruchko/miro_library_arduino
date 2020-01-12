#include "MIROLdr.h"

#define NUM 0
#define TYPE 1

#define LDR_PINS_COUNT 1

#define LDR_PCOUNT 1
#define LDR_VALUE 1

//using namespace miro;

const char _const_dev_name[] = "LDR";

void MIROLdr::Init(byte pin)
{
	this->pins[NUM] = (byte*)malloc(LDR_PINS_COUNT);
	this->pins[TYPE] = (byte*)malloc(LDR_PINS_COUNT);
	this->pins[NUM][0] = pin;
	this->pins[TYPE][0] = INPUT;
	pinMode(pin, INPUT);
}

byte MIROLdr::getPinsCount() { return LDR_PINS_COUNT; };
char* MIROLdr::getName() { return _const_dev_name; }
byte MIROLdr::getParamCount() { return LDR_PCOUNT; }

void MIROLdr::getParam(byte pnum, byte *pvalue)
{
	if (pnum == LDR_VALUE)  *((int*)pvalue) = this->getValue();
}

int MIROLdr::getValue()
{
	return analogRead(this->pins[NUM][0]);
}

float MIROLdr::getVoltage(float vref)
{
	return vref*((analogRead(this->pins[NUM][0]))/1023);
}
