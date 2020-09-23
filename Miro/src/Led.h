#ifndef miro_led_h
#define miro_led_h

#include "Device.h"

class Led : public Device
{
public:
	Led(uint8_t *pins, uint8_t pins_count);
	~Led();
	//void Init(uint8_t* pin, uint8_t pin_count = LED_PINS_COUNT);
	//char *getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
	uint8_t *getParam(uint8_t pnum);
	uint8_t getParamType(uint8_t pnum);

private:
	uint8_t _value;
};

Device *createLED(uint8_t *);
Device *createLED();

void destroyLED(Device *);

uint8_t getDefaultPinLED(uint8_t);
const char *getPinNameLED(uint8_t);

#endif