#pragma once

#include "Chassis.h"
#include "Device.h"

#include "defs.h"
#include "config.h"
#include "default_config.h"

//namespace miro {

class Robot {
public:

	Robot(byte *PWM_pins, byte *DIR_pins);
#if defined(ENCODERS_ON)
	Robot(byte *PWM_pins, byte *DIR_pins, byte *ENCODER_pins);
#endif
	~Robot();
	
	Chassis chassis;
	
	void Sync();

	/*подключение устройства к роботу*/
	int attachDevice(Device *dev);

	int dettachDevice(Device *dev);
	int dettachDevice(byte DeviceIndex);

    Device* getDeviceByIndex(byte DeviceIndex);
    byte getDeviceCount();
	
protected:

	Device* _devices[ROBOT_MAX_DEVICES];
	byte _device_count;
};

//} // end namespace
