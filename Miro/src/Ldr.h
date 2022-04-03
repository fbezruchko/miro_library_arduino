#ifndef miro_ldr_h
#define miro_ldr_h

#include "Device.h"

class Ldr : public Device
{
public:
	Ldr(uint8_t *pins, uint8_t pins_count);
	~Ldr();
	//void Init(uint8_t* pin, uint8_t pin_count = LDR_PINS_COUNT);
	//char *getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
	uint8_t *getParam(uint8_t pnum);
	uint8_t getParamType(uint8_t pnum);

private:
	uint16_t _value;
};

Device *createLDR(uint8_t *);
Device *createLDR();

void destroyLDR(Device *);

uint8_t getDefaultPinLDR(uint8_t);
const char *getPinNameLDR(uint8_t);

#endif