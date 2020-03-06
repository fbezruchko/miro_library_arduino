#ifndef miro_led_h
#define miro_led_h

#include "Device.h"

//namespace miro {
	
#define LED_PINS_COUNT 1
#define LED_PCOUNT 1
#define LED_VALUE 1

class Led : public Device {
public:
	Led(uint8_t* pin, uint8_t pin_count = 1);
	~Led();
	//void Init(uint8_t* pin, uint8_t pin_count = LED_PINS_COUNT);
	char* getName();
	uint8_t getPinsCount();
	uint8_t getParamCount();
	void setParam(uint8_t pnum, uint8_t *pvalue);
    void getParam(uint8_t pnum, uint8_t *pvalue);
	
private:
    uint8_t value;
};

Device* CreateLED(uint8_t *, uint8_t);
void DestroyLED(Device*);

//}

#endif