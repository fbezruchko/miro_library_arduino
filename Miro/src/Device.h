#ifndef device_h
#define device_h

#include <stdint.h>

//namespace miro {

//#include "device_types_enum.h"

class Device {
public:
	Device(uint8_t* pins, uint8_t pins_count);
	~Device();
	//virtual void Init(uint8_t* pin, uint8_t pin_count);
	virtual char* getName();
	virtual uint8_t getPinsCount();
	virtual uint8_t getParamCount();
	virtual void setParam(uint8_t pnum, uint8_t *pvalue);
    virtual void getParam(uint8_t pnum, uint8_t *pvalue);
	virtual void Sync();
	uint8_t getPinNum(uint8_t pin_num);
	uint8_t getPinType(uint8_t pin_num);

protected:	
	uint8_t *_pins[2]; //Pins array. First int8_t - pin number, second int8_t - type of the pin (INPUT/OUTPUT)
};

//} //end namespace

#endif
