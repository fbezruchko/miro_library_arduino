#include "Miro.h"

#define MB_PI (3.14159265)
#define MB_PI2ANG (57.2957795)

void Miro::Init()
{
	this->chassis.Init();
    this->_device_count = 0;
}

void Miro::Sync()
{
	this->chassis.Sync();
	for (byte i = 0; i < this->_device_count; i++)
	{
		this->devices[i]->Sync();
	}
}

int Miro::MoveDist(float lin_speed, float ang_speed, float dist)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = MB_PI2ANG * (lin_speed - (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * (lin_speed + (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;

	float _wheelSetAng[WHEEL_COUNT];
	_wheelSetAng[RIGHT] = _wheelSetAngSpeed[RIGHT] * dist / lin_speed;
	_wheelSetAng[LEFT] = _wheelSetAngSpeed[LEFT] * dist / lin_speed;

	/*
	Serial.print("L speed: ");
	Serial.println(_wheelSetAngSpeed[LEFT]);
	Serial.print("L angle: ");
	Serial.println(_wheelSetAng[LEFT]);
	Serial.print("R speed: ");
	Serial.println(_wheelSetAngSpeed[RIGHT]);
	Serial.print("R angle: ");
	Serial.println(_wheelSetAng[RIGHT]);
	Serial.println();
	*/

	int result = this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng);
	return result;
}

int Miro::RotateAng(float ang_speed, float ang)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = -MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;

	float _wheelSetAng[WHEEL_COUNT];
	_wheelSetAng[RIGHT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	_wheelSetAng[LEFT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	//this->_wheelSetAng[LEFT] = (360.0 * ROBOT_DIAMETER * ang) / (2.0 * MB_PI2ANG * 2.0 * MB_PI * WHEEL_RADIUS);

	/*
	Serial.print("L speed: ");
	Serial.println(_wheelSetAngSpeed[LEFT]);
	Serial.print("L angle: ");
	Serial.println(_wheelSetAng[LEFT]);
	Serial.print("R speed: ");
	Serial.println(_wheelSetAngSpeed[RIGHT]);
	Serial.print("R angle: ");
	Serial.println(_wheelSetAng[RIGHT]);
	Serial.println();
	*/

	int result = this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng);
	return result;
}

float Miro::getAngSpeedRad()
{
	return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) / ROBOT_DIAMETER);
}

float Miro::getAngSpeed()
{
	return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) * MB_PI2ANG / ROBOT_DIAMETER);
}

float Miro::getLinSpeed()
{
	return ((this->chassis.wheelGetLinSpeed(RIGHT) + this->chassis.wheelGetLinSpeed(LEFT)) / 2.0);
}

float Miro::getPath()
{
	return ((this->chassis.wheelGetPath(RIGHT) + this->chassis.wheelGetPath(LEFT)) / 2.0);
}

int Miro::attachDevice(Device *dev)
{
	if (dev == nullptr) return -1;
	if (this->_device_count == MAX_DEVICES) return -3;
	
	this->devices[this->_device_count] = dev;
	this->_device_count++;

	return 0;
}

int Miro::dettachDevice(Device *dev)
{
	if (dev == nullptr) return -1;

	for (byte d = 0; d < this->_device_count; d++)
	{
		if (this->devices[d] == dev)
		{
			return this->dettachDevice(d);
		}
	}

	return -2; //Такого устройства нет среди подключенных к роботу
}

int Miro::dettachDevice(byte DeviceIndex)
{
	if (DeviceIndex > this->_device_count) return -2;

	for (byte d = DeviceIndex; d < this->_device_count; d++)
	{
		this->devices[d] = this->devices[d + 1];
	}
	this->_device_count--;

	return 0;
}
/*
char* Miro::GetDeviceNameByIndex(byte DeviceIndex)
{
    if ((DeviceIndex >= 0) && (DeviceIndex < (this->_device_count - 1)))
    {
        return (this->devices[DeviceIndex]->_dev_name);
    }
    else return NULL; 
}
*/
Device* Miro::GetDeviceByIndex(byte DeviceIndex)
{
    if ((DeviceIndex >= 0) && (DeviceIndex < this->_device_count))
    {
        return this->devices[DeviceIndex];
    }
    else return NULL;
}
/*
byte Miro::GetParamCountByDeviceIndex(byte DeviceIndex)
{
    if ((DeviceIndex >= 0) && (DeviceIndex < this->_device_count))
    {
        return (this->devices[DeviceIndex]->_param_count);
    }
    else return -1;
}
*/
byte Miro::GetDeviceCount()
{
    return this->_device_count;
}
