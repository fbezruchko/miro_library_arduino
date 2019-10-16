#ifndef device_h
#define device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace miro {

class Device
{
public:
	virtual void Sync();
	virtual void SetParam(byte pnum, byte *pvalue);
    virtual void GetParam(byte pnum, byte *pvalue);
	virtual byte GetPinsCount();
	virtual char* GetName();
	virtual byte GetParamCount();

protected:	
	byte *pins[2]; //Массив пинов. Первый байт определяет номер пина, второй - тип (INPUT/OUTPUT)
};

} //end namespace

#endif
