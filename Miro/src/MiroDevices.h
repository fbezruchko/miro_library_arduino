#ifndef MiroDevices_h
#define MiroDevices_h

#include "Device.h"

class MiroDevices
{
public:
	Device *createDevice(uint8_t id, uint8_t *pins);
	Device *createDevice(uint8_t id);
	void destroyDevice(Device *device);

	uint8_t getDeviceId(char *name);

	uint8_t getDevicePinsCount(uint8_t id);

	const char *getDeviceName(uint8_t id);
};

#endif