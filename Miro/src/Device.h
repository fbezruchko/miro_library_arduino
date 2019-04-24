#ifndef device_h
#define device_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define MAX_PINS 8 //Максимальное число пинов, которое может подключаться к контроллеру роботу от одного устройства
#define NUM 0
#define TYPE 1

class Device
{
public:
	virtual void Sync();
	byte pins_count;
	byte pins[MAX_PINS][2];//Массив пинов. Первый байт определяет номер пина, второй - тип (INPUT/OUTPUT).
    virtual void SetParam(byte pnum, float pvalue);
    virtual void GetParam(byte pnum, byte *pvalue);
    byte GetParamCount();
    char* GetName();

protected:
    char* _dev_name;
    byte _param_count;
};
#endif
