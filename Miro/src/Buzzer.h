#ifndef miro_buzzer_h
#define miro_buzzer_h

#include "Device.h"

class Buzzer : public Device
{
public:
	Buzzer(uint8_t *pins, uint8_t pins_count);
	~Buzzer();
	//void Init(uint8_t* pin, uint8_t pin_count = BUZZER_PINS_COUNT);
	//char *getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
	uint8_t *getParam(uint8_t pnum);
	uint8_t getParamType(uint8_t pnum);

private:
	uint16_t _frequency;
	uint32_t _duration;
};

Device *createBUZZER(uint8_t *);
Device *createBUZZER();

void destroyBUZZER(Device *);

uint8_t getDefaultPinBUZZER(uint8_t);
const char *getPinNameBUZZER(uint8_t);

#endif