#ifndef miro_hcsr04_h
#define miro_hcsr04_h

#include "Device.h"

class Hcsr04 : public Device
{
public:
    Hcsr04(uint8_t *pins, uint8_t pins_count);
    ~Hcsr04();
    //void Init(uint8_t* pin, uint8_t pin_count = HCSR04_PINS_COUNT);
    //char *getName();
    uint8_t getPinsCount();
    uint8_t getParamCount();
    void setParam(uint8_t pnum, uint8_t *pvalue);
    uint8_t *getParam(uint8_t pnum);
    uint8_t getParamType(uint8_t pnum);

private:
    float _value;
};

Device *createHCSR04(uint8_t *);
Device *createHCSR04();

void destroyHCSR04(Device *);

uint8_t getDefaultPinHCSR04(uint8_t);
const char *getPinNameHCSR04(uint8_t);

#endif