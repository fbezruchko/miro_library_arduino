#include <string.h>

#include "MiroDevices.h"
#include "device_types_defines.h"

Device *MiroDevices::createDevice(uint8_t id, uint8_t *pins)
{
	return DEVICE_CREATOR[id](pins);
}

Device *MiroDevices::createDevice(uint8_t id)
{
	return DEVICE_CREATOR_DEFAULT[id]();
}

void MiroDevices::destroyDevice(Device *device)
{
	uint8_t id = this->getDeviceID((const char *)device->getParam(0));
	DEVICE_DESTROYER[id](device);
}

uint8_t MiroDevices::getDeviceID(const char *name)
{
	uint8_t id;

	for (id = 0; id < (NULLDEVICE + 1); id++)
	{
		if (!strcmp(name, DEVICE_NAMES[id]))
			break;
	}
	return id;
}

uint8_t MiroDevices::getDevicePinsCount(uint8_t id)
{
	return DEVICE_PINS_COUNT[id];
}

uint8_t MiroDevices::getDeviceDefaultPin(uint8_t id, uint8_t pinIndex)
{
	return DEVICE_GET_PIN[id](pinIndex);
}

const char *MiroDevices::getDevicePinName(uint8_t id, uint8_t pinIndex)
{
	return DEVICE_GET_PIN_NAME[id](pinIndex);
}

const char *MiroDevices::getDeviceName(uint8_t id)
{
	return DEVICE_NAMES[id];
}

uint8_t MiroDevices::getDeviceIDsCount()
{
	return MiroDevices::getDeviceID("NULLDEVICE");
}