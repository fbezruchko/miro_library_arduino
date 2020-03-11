#ifndef MIRODevices_h
#define MIRODevices_h

#include "Device.h"

class MIRODevices {
public:

	Device*	CreateDevice(uint8_t id, uint8_t* pins);
	Device* CreateDevice(uint8_t id);
	void DestroyDevice(Device* device);
	
	uint8_t getDeviceId(char* name);
	
	uint8_t getDevicePinsCount(uint8_t id);
	
	char* getDeviceName(uint8_t id);	
};
	
#endif