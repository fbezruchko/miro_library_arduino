#include "MIROServo.h"

#define NUM 0
#define TYPE 1

#define SERVO_PINS_COUNT 1

#define SERVO_PCOUNT 1
#define SERVO_VALUE 1

//using namespace miro;

const char _const_dev_name[] = "SERVO";

void MIROServo::Init(byte pin, byte min_value, byte max_value)
{
	this->pins[NUM] = malloc(SERVO_PINS_COUNT);
	this->pins[TYPE] = malloc(SERVO_PINS_COUNT);
	this->pins[NUM][0] = pin;
	this->pins[TYPE][0] = OUTPUT;
	pinMode(pin, OUTPUT);
	this->_min_value = min_value;
	this->_max_value = max_value;
	this->servo0.attach(pin);
	this->servo0.write(min_value);
	delay(200);
	this->servo0.write(max_value);
	delay(200);
	this->servo0.write(min_value+(max_value - min_value)/2);
}

byte MIROServo::getPinsCount() { return SERVO_PINS_COUNT; };
char* MIROServo::getName() { return _const_dev_name; }
byte MIROServo::getParamCount() { return SERVO_PCOUNT; }

int MIROServo::Pos(byte value)
{
	if ((this->_min_value > value) || (this->_max_value < value)) return -1;
	this->servo0.write(value);
	return 0;
}

void MIROServo::setParam(byte pnum, byte *pvalue)
{
    if (pnum == SERVO_VALUE) this->Pos(*pvalue);
}

void MIROServo::getParam(byte pnum, byte *pvalue)
{
    if (pnum == SERVO_VALUE)  *pvalue = this->servo0.read();
}