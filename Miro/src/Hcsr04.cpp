#include "Arduino.h"

#include "Hcsr04.h"

#define HCSR04_PINS_COUNT 2
#define HCSR04_PCOUNT 1
#define HCSR04_VALUE 1

#define NUM 0
#define TYPE 1

#define TRIG 13
#define ECHO 12

#define MAX_DIST_TIMEOUT 23200 //4(meters) * 5.8

const char _const_dev_name[] = "HCSR04";
const uint8_t _default_pins[] = {TRIG, ECHO};
const char *const _pin_names[HCSR04_PINS_COUNT] = {"TRIG", "ECHO"};

Hcsr04::Hcsr04(uint8_t *pins, uint8_t pins_count = HCSR04_PINS_COUNT) : Device(pins, pins_count)
{
    this->_pins[TYPE][0] = OUTPUT;
    this->_pins[TYPE][1] = INPUT;
    pinMode(this->_pins[NUM][0], OUTPUT);
    pinMode(this->_pins[NUM][1], INPUT);
    digitalWrite(TRIG, LOW);
    this->_value = 0;
}

Hcsr04::~Hcsr04()
{
    //Serial.println("HCSR04 device delete");
}

uint8_t Hcsr04::getPinsCount() { return HCSR04_PINS_COUNT; };
//char *Hcsr04::getName() { return (char *)_const_dev_name; }
uint8_t Hcsr04::getParamCount() { return HCSR04_PCOUNT; }

void Hcsr04::setParam(uint8_t pnum, uint8_t *pvalue)
{
    return;
}

uint8_t *Hcsr04::getParam(uint8_t pnum)
{
    if (pnum == 0)
    {
        return (uint8_t *)_const_dev_name;
    }
    if (pnum == HCSR04_VALUE)
    {
        //digitalWrite(TRIG, LOW);
        //delayMicroseconds(2);
        digitalWrite(this->_pins[NUM][0], HIGH);
        delayMicroseconds(10);
        digitalWrite(this->_pins[NUM][0], LOW);
        this->_value = pulseIn(this->_pins[NUM][1], HIGH, MAX_DIST_TIMEOUT);
        this->_value = this->_value / 5.8;
        return (uint8_t *)(&this->_value);
    }
    else
    {
        return nullptr;
    }
}

uint8_t Hcsr04::getParamType(uint8_t pnum)
{
    if (pnum == 0)
        return PTYPE_CSTRING;
    if (pnum == HCSR04_VALUE)
        return PTYPE_FLOAT;
    return 0;
}

Device *createHCSR04(uint8_t *pins)
{
    Device *d = new Hcsr04(pins);
    return d;
}

Device *createHCSR04()
{
    Device *d = new Hcsr04((uint8_t *)_default_pins);
    return d;
}

void destroyHCSR04(Device *device)
{
    delete ((Hcsr04 *)device);
    return;
}

uint8_t getDefaultPinHCSR04(uint8_t pinIndex)
{
    return _default_pins[pinIndex];
}

const char *getPinNameHCSR04(uint8_t pinIndex)
{
    return _pin_names[pinIndex];
}