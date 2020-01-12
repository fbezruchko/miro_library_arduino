#include "MIROLine.h"

#define NUM 0
#define TYPE 1

#define LINE_PINS_COUNT 1

#define LINE_PCOUNT 1
#define LINE_VALUE 1

//using namespace miro;

const char _const_dev_name[] = "LINE";

void MIROLine::Init(byte pin)
{
	this->pins[NUM] = (byte*)malloc(LINE_PINS_COUNT);
	this->pins[TYPE] = (byte*)malloc(LINE_PINS_COUNT);
	this->pins[NUM][0] = pin;
	this->pins[TYPE][0] = INPUT;
	pinMode(pin, INPUT);
}

byte MIROLine::getPinsCount() { return LINE_PINS_COUNT; };
char* MIROLine::getName() { return _const_dev_name; }
byte MIROLine::getParamCount() { return LINE_PCOUNT; }

void MIROLine::getParam(byte pnum, byte *pvalue)
{
	if (pnum == LINE_VALUE)  *((int*)pvalue) = this->getValue();
}

int MIROLine::getValue()
{
	return analogRead(this->pins[NUM][0]);
}

float MIROLine::getVoltage(float vref)
{
	return vref*((analogRead(this->pins[NUM][0]))/1023);
}
