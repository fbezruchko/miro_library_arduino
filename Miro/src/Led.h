#include "Device.h"

#define LED_VALUE 1

class Led : virtual public Device {
public:
	void Init(byte pin);
	void On(byte value);
	void On();
	void Off();
    void SetParam(byte bnum, float pvalue);
    void GetParam(byte bnum, byte *pvalue);
    const char _const_dev_name[4] = "LED";
private:
    byte led_value;
};
