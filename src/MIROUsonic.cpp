#include "MIROUsonic.h"

#define TRIG_PIN 0
#define ECHO_PIN 1

#define NUM 0
#define TYPE 1

#define USONIC_PINS_COUNT 2

#define USONIC_PCOUNT 2
#define USONIC_VALUE 1
#define USONIC_STATE 2

#define MAX_DIST 4000 //4 метра

//using namespace miro;

const char _const_dev_name[] = "USONIC";

void MIROUsonic::Init(byte trig_pin, byte echo_pin)
{
	this->pins[NUM] = (byte*)malloc(USONIC_PINS_COUNT);
	this->pins[TYPE] = (byte*)malloc(USONIC_PINS_COUNT);
	
	this->pins[NUM][TRIG_PIN] = trig_pin;
	this->pins[TYPE][TRIG_PIN] = OUTPUT;
	this->pins[NUM][ECHO_PIN] = echo_pin;
	this->pins[TYPE][ECHO_PIN] = INPUT;
	
	pinMode(trig_pin, OUTPUT);
	pinMode(echo_pin, INPUT);

	this->_dist = 0;
	this->_mesCount = 0;
	this->_max_dist = MAX_DIST;
	this->_isOn = false;
}

byte MIROUsonic::getPinsCount() { return USONIC_PINS_COUNT; }
char* MIROUsonic::getName() { return _const_dev_name; }
byte MIROUsonic::getParamCount() { return USONIC_PCOUNT; }

void MIROUsonic::On(unsigned int max_dist)
{
	this->_isOn = true;
	this->_max_dist = max_dist;
}

void MIROUsonic::On()
{
	this->_isOn = true;
	this->_max_dist = MAX_DIST;
}

void MIROUsonic::Off()
{
	this->_isOn = false;
}

void MIROUsonic::setParam(byte pnum, byte *pvalue)
{
    if (pnum == USONIC_STATE) this->_isOn = (bool)(*pvalue);
}

void MIROUsonic::getParam(byte pnum, byte *pvalue)
{
    if (pnum == USONIC_STATE)  *pvalue = this->_isOn;
	if (pnum == USONIC_VALUE)  *((int*)pvalue) = this->getDist(MAX_DIST);
}

void MIROUsonic::Sync()
{
	if (this->_isOn)
	{
		digitalWrite(this->pins[NUM][TRIG_PIN], LOW);
		delayMicroseconds(2);
		digitalWrite(this->pins[NUM][TRIG_PIN], HIGH);
		delayMicroseconds(10);
		digitalWrite(this->pins[NUM][TRIG_PIN], LOW);
		this->_dist = pulseIn(this->pins[NUM][ECHO_PIN], HIGH, this->_max_dist * 5.8);
		this->_dist = this->_dist / 5.8;
		if (this->_dist > this->_max_dist) this->_dist = this->_max_dist + 1;
		this->_mesCount++;
	}
}

int MIROUsonic::getDist(unsigned int max_dist)
{
	if (!this->_isOn)
	{
		digitalWrite(this->pins[NUM][TRIG_PIN], LOW);
		delayMicroseconds(2);
		digitalWrite(this->pins[NUM][TRIG_PIN], HIGH);
		delayMicroseconds(10);
		digitalWrite(this->pins[NUM][TRIG_PIN], LOW);
		this->_dist = pulseIn(this->pins[NUM][ECHO_PIN], HIGH, max_dist * 5.8);
		this->_dist = this->_dist / 5.8;
		if ((this->_dist > max_dist) || (this->_dist == 0)) return -1;
		this->_mesCount++;
	}

	return this->_dist;
}
