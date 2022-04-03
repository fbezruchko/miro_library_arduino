#ifndef miro_line_h
#define miro_line_h

#include "Device.h"

class Line : public Device
{
public:
	Line(uint8_t *pins, uint8_t pins_count);
	~Line();
	//void Init(uint8_t* pin, uint8_t pin_count = LINE_PINS_COUNT);
	//char *getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
	uint8_t *getParam(uint8_t pnum);
	uint8_t getParamType(uint8_t pnum);

private:
	uint16_t _value;
};

Device *createLINE(uint8_t *);
Device *createLINE();

void destroyLINE(Device *);

uint8_t getDefaultPinLINE(uint8_t);
const char *getPinNameLINE(uint8_t);

#endif