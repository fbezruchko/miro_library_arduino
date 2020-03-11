#include <string.h>

#include "MIRODevices.h"
#include "device_types_defines.h"

Device* MIRODevices::CreateDevice(uint8_t id, uint8_t* pins)
{
	return DEVICE_CREATOR[id](pins);
}

Device* MIRODevices::CreateDevice(uint8_t id)
{
	return DEVICE_CREATOR_DEFAULT[id]();
}

void MIRODevices::DestroyDevice(Device* device)
{
	uint8_t id = this->getDeviceId(device->getName());
	DEVICE_DESTROYER[id](device);
}

uint8_t MIRODevices::getDeviceId(char* name)
{
	uint8_t id;
	
	for (id = 0; id < (NULLDEVICE + 1); id++)
	{
		if (!strcmp(name, DEVICE_NAMES[id])) break;
	}
	return id;
}

uint8_t MIRODevices::getDevicePinsCount(uint8_t id)
{
	return DEVICE_PINS_COUNT[id];
}

char* MIRODevices::getDeviceName(uint8_t id)
{
	return DEVICE_NAMES[id];
}