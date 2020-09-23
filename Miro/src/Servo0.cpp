#include "Arduino.h"

#include "Servo0.h"

#define SERVO_PINS_COUNT 1
#define SERVO_PCOUNT 1
#define SERVO_VALUE 1

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "SERVO";
const uint8_t _default_pins[] = {8};
const char *const _pin_names[SERVO_PINS_COUNT] = {"PPM"};

Servo0::Servo0(uint8_t *pins, uint8_t pins_count = SERVO_PINS_COUNT) : Device(pins, pins_count)
{
	this->_pins[TYPE][0] = OUTPUT;
	pinMode(this->_pins[NUM][0], OUTPUT);
	this->_servo.attach(this->_pins[NUM][0]);
	this->_value = 0;
}

Servo0::~Servo0()
{
	//Serial.println("SERVO device delete");
}

uint8_t Servo0::getPinsCount() { return SERVO_PINS_COUNT; };
//char *Servo0::getName() { return (char *)_const_dev_name; }
uint8_t Servo0::getParamCount() { return SERVO_PCOUNT; }

void Servo0::setParam(uint8_t pnum, uint8_t *pvalue)
{
	if (pnum == SERVO_VALUE)
	{
		this->_value = *pvalue % 180;
		this->_servo.write(this->_value);
		//delay(5);
	}
}

uint8_t *Servo0::getParam(uint8_t pnum)
{
	if (pnum == 0)
	{
		return (uint8_t *)_const_dev_name;
	}
	if (pnum == SERVO_VALUE)
	{
		return (uint8_t *)(&this->_value);
	}
	else
	{
		return nullptr;
	}
}

uint8_t Servo0::getParamType(uint8_t pnum)
{
	if (pnum == 0)
		return PTYPE_CSTRING;
	if (pnum == SERVO_VALUE)
		return PTYPE_INT16;
	return 0;
}

Device *createSERVO(uint8_t *pins)
{
	Device *d = new Servo0(pins);
	return d;
}

Device *createSERVO()
{
	Device *d = new Servo0((uint8_t *)_default_pins);
	return d;
}

void destroySERVO(Device *device)
{
	delete ((Servo0 *)device);
	return;
}

uint8_t getDefaultPinSERVO(uint8_t pinIndex)
{
	return _default_pins[pinIndex];
}

const char *getPinNameSERVO(uint8_t pinIndex)
{
	return _pin_names[pinIndex];
}