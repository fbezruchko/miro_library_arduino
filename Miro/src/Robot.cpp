#include "Robot.h"

//using namespace miro;

//========================================== C
Robot::Robot(uint8_t *PWM_pins, uint8_t *DIR_pins)
{
	_init(PWM_pins, DIR_pins);
}

void Robot::_init(uint8_t *PWM_pins, uint8_t *DIR_pins)
{
	this->chassis._init(PWM_pins, DIR_pins);
	this->_device_count = 0;
}

#if defined(ENCODERS_ON)
//========================================== C
Robot::Robot(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins)
{
	_init(PWM_pins, DIR_pins, ENCODER_pins);
}

void Robot::_init(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins)
{
	this->chassis._init(PWM_pins, DIR_pins, ENCODER_pins);
	this->_device_count = 0;
}
#endif // ENCODERS_ON

//========================================== D
Robot::~Robot()
{
}

//========================================== Sync
void Robot::sync()
{
	this->chassis.sync();
	for (int8_t i = this->_device_count - 1; i >= 0; --i)
		this->_devices[i]->sync();
}

//========================================== attachDevice
int Robot::attachDevice(Device *dev)
{
	if (dev == nullptr)
		return -1;
	if (this->_device_count == ROBOT_MAX_DEVICES)
		return -3;

	this->_devices[this->_device_count] = dev;
	this->_device_count++;

	return 0;
}

//========================================== dettachDevice
int Robot::dettachDevice(Device *dev)
{
	if (dev == nullptr)
		return -1;
	for (uint8_t d = 0; d < this->_device_count; d++)
	{
		if (this->_devices[d] == dev)
			return this->dettachDevice(d);
	}

	return -2; // Device not found
}

//========================================== dettachDevice
int Robot::dettachDevice(uint8_t deviceIndex)
{
	if ((deviceIndex + 1) > this->_device_count)
		return -2;
	for (uint8_t d = deviceIndex; (d + 1) < this->_device_count; d++)
		this->_devices[d] = this->_devices[d + 1];

	this->_device_count--;

	return 0;
}

//========================================== GetDeviceByIndex
Device *Robot::getDeviceByIndex(uint8_t deviceIndex)
{
	if (deviceIndex < this->_device_count)
	{
		return this->_devices[deviceIndex];
	}
	else
		return nullptr;
}

//========================================== GetDeviceCount
uint8_t Robot::getDeviceCount() { return this->_device_count; }
