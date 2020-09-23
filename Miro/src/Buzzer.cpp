#include "Arduino.h"

#include "Buzzer.h"

#define BUZZER_PINS_COUNT 1
#define BUZZER_PCOUNT 2
#define BUZZER_FREQUENCY 1
#define BUZZER_DURATION 2

#define NUM 0
#define TYPE 1

const char _const_dev_name[] = "BUZZER";
const uint8_t _default_pins[] = {8};
const char *const _pin_names[BUZZER_PINS_COUNT] = {"PWM"};

Buzzer::Buzzer(uint8_t *pins, uint8_t pins_count = BUZZER_PINS_COUNT) : Device(pins, pins_count)
{
	this->_pins[TYPE][0] = OUTPUT;
	pinMode(this->_pins[NUM][0], OUTPUT);
	this->_frequency = 0;
	this->_duration = 0;
}

Buzzer::~Buzzer()
{
	//Serial.println("BUZZER device delete");
}

uint8_t Buzzer::getPinsCount() { return BUZZER_PINS_COUNT; };
//char *Buzzer::getName() { return (char *)_const_dev_name; }
uint8_t Buzzer::getParamCount() { return BUZZER_PCOUNT; }

void Buzzer::setParam(uint8_t pnum, uint8_t *pvalue)
{
	if (pnum == BUZZER_FREQUENCY)
	{
		this->_frequency = *((uint16_t *)pvalue);
		tone(this->_pins[NUM][0], this->_frequency, this->_duration);
	}
	if (pnum == BUZZER_DURATION)
	{
		this->_duration = *((uint32_t *)pvalue);
		tone(this->_pins[NUM][0], this->_frequency, this->_duration);
	}
}

uint8_t *Buzzer::getParam(uint8_t pnum)
{
	if (pnum == 0)
	{
		return (uint8_t *)_const_dev_name;
	}
	if (pnum == BUZZER_FREQUENCY)
	{
		return (uint8_t *)(&this->_frequency);
	}
	else if (pnum == BUZZER_DURATION)
	{
		return (uint8_t *)(&this->_duration);
	}
	else
	{
		return nullptr;
	}
}

uint8_t Buzzer::getParamType(uint8_t pnum)
{
	if (pnum == 0)
		return PTYPE_CSTRING;
	if (pnum == BUZZER_FREQUENCY)
		return PTYPE_UINT16;
	if (pnum == BUZZER_DURATION)
		return PTYPE_UINT32;
	return 0;
}

Device *createBUZZER(uint8_t *pins)
{
	Device *d = new Buzzer(pins);
	return d;
}

Device *createBUZZER()
{
	Device *d = new Buzzer((uint8_t *)_default_pins);
	return d;
}

void destroyBUZZER(Device *device)
{
	delete ((Buzzer *)device);
	return;
}

uint8_t getDefaultPinBUZZER(uint8_t pinIndex)
{
	return _default_pins[pinIndex];
}

const char *getPinNameBUZZER(uint8_t pinIndex)
{
	return _pin_names[pinIndex];
}