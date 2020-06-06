#ifndef robot_h
#define robot_h

#include "Chassis.h"
#include "Device.h"

#include "defs.h"
#include "config.h"
#include "default_config.h"

//namespace miro {

class Robot {
public:

	Robot() {};
	Robot(uint8_t *PWM_pins, uint8_t *DIR_pins);
#if defined(ENCODERS_ON)
	Robot(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins);
#endif  // ENCODERS_ON
	~Robot();
	
	Chassis chassis;
	
	void sync();

	/*connect device to robot*/
	int attachDevice(Device *dev);

	int dettachDevice(Device *dev);
	int dettachDevice(uint8_t deviceIndex);

    Device* getDeviceByIndex(uint8_t deviceIndex);
    uint8_t getDeviceCount();
	
protected:

	void _init(uint8_t *PWM_pins, uint8_t *DIR_pins);
#if defined(ENCODERS_ON)
	void _init(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins);
#endif  // ENCODERS_ON

	Device* _devices[ROBOT_MAX_DEVICES];
	uint8_t _device_count;
};

//} // end namespace

#endif