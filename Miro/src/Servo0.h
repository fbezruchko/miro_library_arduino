#ifndef miro_servo0_h
#define miro_servo0_h

#include <Arduino.h>
#include "Servo.h"
#include "Device.h"

class Servo0 : public Device
{
public:
	Servo0(uint8_t *pins, uint8_t pins_count);
	~Servo0();
	//void Init(uint8_t* pin, uint8_t pin_count = SERVO_PINS_COUNT);
	//char *getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
	uint8_t *getParam(uint8_t pnum);
	uint8_t getParamType(uint8_t pnum);

private:
	int16_t _value;
	Servo _servo;
};

Device *createSERVO(uint8_t *);
Device *createSERVO();

void destroySERVO(Device *);

uint8_t getDefaultPinSERVO(uint8_t);
const char *getPinNameSERVO(uint8_t);

#endif