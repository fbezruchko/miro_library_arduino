#ifndef device_h
#define device_h

#include <stdint.h>

#define PTYPE_VOID 0
#define PTYPE_BOOL 1
#define PTYPE_CHAR 2
#define PTYPE_CSTRING 3
#define PTYPE_UINT8 4
#define PTYPE_INT8 5
#define PTYPE_UINT16 6
#define PTYPE_INT16 7
#define PTYPE_UINT32 8
#define PTYPE_INT32 9
#define PTYPE_FLOAT 10

class Device
{
public:
	Device(uint8_t *pins, uint8_t pins_count);
	~Device();
	//virtual void Init(uint8_t* pin, uint8_t pin_count);
	//virtual char *getName();
	virtual uint8_t getPinsCount();
	virtual uint8_t getParamCount();
	virtual void setParam(uint8_t pnum, uint8_t *pvalue);
	virtual uint8_t *getParam(uint8_t pnum);
	virtual uint8_t getParamType(uint8_t pnum);
	virtual void sync();
	uint8_t getPinNum(uint8_t pin_num);
	uint8_t getPinType(uint8_t pin_num);

protected:
	uint8_t *_pins[2]; //Pins array. First uint8_t - pin number, second uint8_t - type of the pin (INPUT/OUTPUT)
};

#endif
