#include "Device.h"

class Ldr : virtual public Device {
public:
	void Init(byte pin);
	float getVoltage(float vref);
	int getValue();

private:
	byte pin;
};
