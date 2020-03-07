#ifndef miro_led_h
#define miro_led_h

#include "Device.h"

//namespace miro {


class Led : public Device {
public:
	Led(uint8_t* pins, uint8_t pins_count);
	~Led();
	//void Init(uint8_t* pin, uint8_t pin_count = LED_PINS_COUNT);
	char* getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
    void getParam(uint8_t pnum, uint8_t *pvalue);
	
private:
    uint8_t _value;
};

Device* CreateLED(uint8_t *);
Device* CreateLED();

void DestroyLED(Device*);

//}

#endif