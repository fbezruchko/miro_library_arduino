#include "Miro.h"
#include "defs.h"

using namespace miro;

//========================================== C
Miro::Miro(byte *PWM_pins, byte *DIR_pins) : Robot(PWM_pins, DIR_pins)
{
}

#if defined(ENCODERS_ON)
//========================================== C
Miro::Miro(byte *PWM_pins, byte *DIR_pins, byte *ENCODER_pins) : Robot(PWM_pins, DIR_pins, ENCODER_pins)
{
	this->_device_count = 0;
}
#endif

//========================================== D
Miro::~Miro()
{
}

//========================================== MovePWMTime
int Miro::movePWMTime(int PWM_lin_speed, int PWM_angle_speed, unsigned long time)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2;
	_wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;

	return this->chassis.wheelRotatePWMTime(_wheelSetPWM, time);
}

//========================================== MovePWM
int Miro::rotatePWM(int PWM_speed)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = -PWM_speed;
	_wheelSetPWM[RIGHT] = PWM_speed;

	return this->chassis.wheelRotatePWM(_wheelSetPWM);
}

//========================================== MovePWMTime
int Miro::rotatePWMTime(int PWM_speed, unsigned long time)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = -PWM_speed;
	_wheelSetPWM[RIGHT] = PWM_speed;

	return this->chassis.wheelRotatePWMTime(_wheelSetPWM, time);
}

//========================================== MovePWM
int Miro::movePWM(int PWM_lin_speed, int PWM_angle_speed)
{
	int _wheelSetPWM[WHEEL_COUNT];
	_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2;
	_wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;

	return this->chassis.wheelRotatePWM(_wheelSetPWM);
}

//==========================================
//==========================================
//==========================================

#if defined(ENCODERS_ON)
//========================================== MoveDist
int Miro::moveDist(float lin_speed, float ang_speed, float dist, bool en_break)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = MIRO_PI2ANG * (lin_speed - (ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MIRO_PI2ANG * (lin_speed + (ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;

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

	return this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng, en_break);
}

//========================================== Move
int Miro::move(float lin_speed, float ang_speed)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = MIRO_PI2ANG * (lin_speed - (ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MIRO_PI2ANG * (lin_speed + (ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;
	
	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAng[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.println();

	return this->chassis.wheelRotate(_wheelSetAngSpeed);
}

//========================================== RotateAng
int Miro::rotateAng(float ang_speed, float ang, bool en_break)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = -(ROBOT_DIAMETER * ang_speed) / (2.0 * WHEEL_RADIUS);
	_wheelSetAngSpeed[RIGHT] = (ROBOT_DIAMETER * ang_speed) / (2.0 * WHEEL_RADIUS);

	float _wheelSetAng[WHEEL_COUNT];
	_wheelSetAng[RIGHT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	_wheelSetAng[LEFT] = (ROBOT_DIAMETER * ang) / (2.0 * WHEEL_RADIUS);
	//this->_wheelSetAng[LEFT] = (360.0 * ROBOT_DIAMETER * ang) / (2.0 * MIRO_PI2ANG * 2.0 * MIRO_PI * WHEEL_RADIUS);

	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAngSpeed[LEFT]);
	// Serial.print(F("L angle: "));
	// Serial.println(_wheelSetAng[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.print(F("R angle: "));
	// Serial.println(_wheelSetAng[RIGHT]);
	// Serial.println();

	return this->chassis.wheelRotateAng(_wheelSetAngSpeed, _wheelSetAng, en_break);
}

//========================================== Rotate
int Miro::rotate(float ang_speed)
{
	float _wheelSetAngSpeed[WHEEL_COUNT];
	_wheelSetAngSpeed[LEFT] = -MIRO_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;
	_wheelSetAngSpeed[RIGHT] = MIRO_PI2ANG * ((ROBOT_DIAMETER * ang_speed / (2 * MIRO_PI2ANG))) / WHEEL_RADIUS;

	// Serial.print(F("L speed: "));
	// Serial.println(_wheelSetAngSpeed[LEFT]);
	// Serial.print(F("R speed: "));
	// Serial.println(_wheelSetAngSpeed[RIGHT]);
	// Serial.println();

	return this->chassis.wheelRotate(_wheelSetAngSpeed);
}

//========================================== getAngSpeedRad
float Miro::getAngSpeedRad() { return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) / ROBOT_DIAMETER); }

//========================================== getAngSpeed
float Miro::getAngSpeed() { return ((this->chassis.wheelGetLinSpeed(RIGHT) - this->chassis.wheelGetLinSpeed(LEFT)) * (MIRO_PI2ANG / ROBOT_DIAMETER)); }

//========================================== getLinSpeed
float Miro::getLinSpeed() { return ((this->chassis.wheelGetLinSpeed(RIGHT) + this->chassis.wheelGetLinSpeed(LEFT)) / 2.0); }

//========================================== getPath
float Miro::getPath() { return ((this->chassis.wheelGetPath(RIGHT) + this->chassis.wheelGetPath(LEFT)) / 2.0); }

//========================================== getMaxLinSpeed
float Miro::getMaxLinSpeed()
{
	unsigned long min_wheel_speed_max = this->chassis.getWheelTableValue(0, SPEED, WHEEL_TABLE_SIZE - 1);
	for (char w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->chassis.getWheelTableValue(w, SPEED, WHEEL_TABLE_SIZE - 1) < min_wheel_speed_max) min_wheel_speed_max = this->chassis.getWheelTableValue(w, SPEED, WHEEL_TABLE_SIZE - 1);
	}
	return (min_wheel_speed_max / 180.0) * MIRO_PI * WHEEL_RADIUS;
}

//========================================== getMinLinSpeed
float Miro::getMinLinSpeed()
{
	unsigned long min_wheel_speed_min = this->chassis.getWheelTableValue(0, SPEED, 0);
	for (char w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->chassis.getWheelTableValue(w, SPEED, 0) > min_wheel_speed_min) min_wheel_speed_min = this->chassis.getWheelTableValue(w, SPEED, 0);
	}
	return (min_wheel_speed_min / 180.0) * MIRO_PI * WHEEL_RADIUS;
}

//========================================== getOptLinSpeed
float Miro::getOptLinSpeed()
{
	unsigned long opt_wheel_speed_min = this->chassis.getWheelTableValue(0, SPEED, WHEEL_TABLE_SIZE/4);
	for (char w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->chassis.getWheelTableValue(w, SPEED, WHEEL_TABLE_SIZE/2) < opt_wheel_speed_min) opt_wheel_speed_min = this->chassis.getWheelTableValue(w, SPEED, WHEEL_TABLE_SIZE/2);
	}
	return (opt_wheel_speed_min / 180.0) * MIRO_PI * WHEEL_RADIUS;
}

//========================================== getMaxLinSpeed
float Miro::getMaxAngSpeed()
{
	return (2 * MIRO_PI2ANG * (this->getMaxLinSpeed()) / ROBOT_DIAMETER);
}

//========================================== getMaxLinSpeed
float Miro::getMinAngSpeed()
{
	return (1+ 2 * MIRO_PI2ANG * (this->getMinLinSpeed()) / ROBOT_DIAMETER);
}

//========================================== getMaxLinSpeed
float Miro::getOptAngSpeed()
{
	return (2 * MIRO_PI2ANG * (this->getOptLinSpeed()) / ROBOT_DIAMETER);
}

#endif // ENCODERS_ON