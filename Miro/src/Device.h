#ifndef device_h
#define device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//namespace miro {

class Device
{
public:
	virtual void Sync();
	virtual void setParam(byte pnum, byte *pvalue);
    virtual void getParam(byte pnum, byte *pvalue);
	virtual byte getPinsCount();
	virtual char* getName();
	virtual byte getParamCount();

protected:	
	byte *pins[2]; //Pins array. First byte - pin number, second byte - type of the pin (INPUT/OUTPUT)
};

//} //end namespace

#endif
