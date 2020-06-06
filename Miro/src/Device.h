#ifndef device_h
#define device_h

#include <stdint.h>

class Device
{
public:
	Device(uint8_t *pins, uint8_t pins_count);
	~Device();
	//virtual void Init(uint8_t* pin, uint8_t pin_count);
	virtual char *getName();
	virtual uint8_t getPinsCount();
	virtual uint8_t getParamCount();
	virtual void setParam(uint8_t pnum, uint8_t *pvalue);
	virtual uint8_t *getParam(uint8_t pnum, uint8_t *pvalue);
	virtual void sync();
	uint8_t getPinNum(uint8_t pin_num);
	uint8_t getPinType(uint8_t pin_num);

protected:
	uint8_t *_pins[2]; //Pins array. First int8_t - pin number, second int8_t - type of the pin (INPUT/OUTPUT)
};

#endif
