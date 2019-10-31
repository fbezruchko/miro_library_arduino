#include "Miro.h"

#define LEFT 0
#define RIGHT 1

using namespace miro;

//========================================== Init
Miro::Miro(byte *PWM_pins, byte *DIR_pins)
{
	this->chassis.Init(PWM_pins, DIR_pins);
    this->_device_count = 0;
}

//========================================== Sync
void Miro::Sync()
{
	this->chassis.Sync();
	
	for (byte i = 0; i < this->_device_count; i++) this->devices[i]->Sync();
}

//========================================== MovePWMTime
int Miro::MovePWMTime(int PWM_lin_speed, int PWM_angle_speed, unsigned long time)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2;
	_wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;

	int result = this->chassis.wheelRotatePWMTime(_wheelSetPWM, time);
	return result;
}

//========================================== MovePWM
int Miro::RotatePWM(int PWM_speed)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = -PWM_speed;
	_wheelSetPWM[RIGHT] = PWM_speed;

	int result = this->chassis.wheelRotatePWM(_wheelSetPWM);
	return result;
}

//========================================== MovePWMTime
int Miro::RotatePWMTime(int PWM_speed, unsigned long time)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = -PWM_speed;
	_wheelSetPWM[RIGHT] = PWM_speed;

	int result = this->chassis.wheelRotatePWMTime(_wheelSetPWM, time);
	return result;
}

//========================================== MovePWM
int Miro::MovePWM(int PWM_lin_speed, int PWM_angle_speed)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2;
	_wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;

	int result = this->chassis.wheelRotatePWM(_wheelSetPWM);
	return result;
}

//========================================== attachDevice
int Miro::attachDevice(Device *dev)
{
	if (dev == nullptr) return -1;
	if (this->_device_count == MIRO_MAX_DEVICES) return -3;
	
	this->devices[this->_device_count] = dev;
	this->_device_count++;

	return 0;
}

//========================================== dettachDevice
int Miro::dettachDevice(Device *dev)
{
	if (dev == nullptr) return -1;

	for (byte d = 0; d < this->_device_count; d++)
	{
		if (this->devices[d] == dev) return this->dettachDevice(d);
	}

	return -2; //Такого устройства нет среди подключенных к роботу
}

//========================================== dettachDevice
int Miro::dettachDevice(byte DeviceIndex)
{
	if (DeviceIndex > this->_device_count) return -2;

	for (byte d = DeviceIndex; d < this->_device_count; d++) this->devices[d] = this->devices[d + 1];
	
	this->_device_count--;

	return 0;
}

//========================================== GetDeviceByIndex
Device* Miro::GetDeviceByIndex(byte DeviceIndex)
{
    if ((DeviceIndex >= 0) && (DeviceIndex < this->_device_count))
    {
        return this->devices[DeviceIndex];
    }
    else return NULL;
}

//========================================== GetDeviceCount
byte Miro::GetDeviceCount() { return this->_device_count; }

//==========================================
//==========================================
//==========================================

#if defined (CHASSIS_W_ENCODERS)

//========================================== MoveDist
int Miro::MoveDist(float lin_speed, float ang_speed, float dist, bool en_break)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = MB_PI2ANG * (lin_speed - (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * (lin_speed + (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;

	float _wheelSetAng[WHEEL_COUNT];
	_wheelSetAng[RIGHT] = _wheelSetAngSpeed[RIGHT] * dist / lin_speed;
	_wheelSetAng[LEFT] = _wheelSetAngSpeed[LEFT] * dist / lin_speed;
	
	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAngSpeed[LEFT]);
	// Serial.print(F("L angle: "));
	// Serial.println(_wheelSetAng[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.print(F("R angle: "));
	// Serial.println(_wheelSetAng[RIGHT]);
	// Serial.println();

	int result = this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng, en_break);
	return result;
}

//========================================== Move
int Miro::Move(float lin_speed, float ang_speed)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = MB_PI2ANG * (lin_speed - (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * (lin_speed + (ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	
	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAng[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.println();

	int result = this->chassis.wheelRotate(_wheelSetAngSpeed);
	return result;
}

//========================================== RotateAng
int Miro::RotateAng(float ang_speed, float ang, bool en_break)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = -MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;

	float _wheelSetAng[WHEEL_COUNT];
	_wheelSetAng[RIGHT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	_wheelSetAng[LEFT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	//this->_wheelSetAng[LEFT] = (360.0 * ROBOT_DIAMETER * ang) / (2.0 * MB_PI2ANG * 2.0 * MB_PI * WHEEL_RADIUS);

	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAngSpeed[LEFT]);
	// Serial.print(F("L angle: "));
	// Serial.println(_wheelSetAng[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.print(F("R angle: "));
	// Serial.println(_wheelSetAng[RIGHT]);
	// Serial.println();

	int result = this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng, en_break);
	return result;
}

//========================================== Rotate
int Miro::Rotate(float ang_speed)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = -MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MB_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MB_PI2ANG))) / WHEEL_RADIUS;

	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAngSpeed[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.println();

	int result = this->chassis.wheelRotate(_wheelSetAngSpeed);
	return result;
}

//========================================== getAngSpeedRad
float Miro::getAngSpeedRad() { return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) / ROBOT_DIAMETER); }

//========================================== getAngSpeed
float Miro::getAngSpeed() { return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) * (MB_PI2ANG / ROBOT_DIAMETER)); }

//========================================== getLinSpeed
float Miro::getLinSpeed() { return ((this->chassis.wheelGetLinSpeed(RIGHT) + this->chassis.wheelGetLinSpeed(LEFT)) / 2.0); }

//========================================== getPath
float Miro::getPath() { return ((this->chassis.wheelGetPath(RIGHT) + this->chassis.wheelGetPath(LEFT)) / 2.0); }

#endif // CHASSIS_W_ENCODERS