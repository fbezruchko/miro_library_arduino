#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "defs.h"
#include "config.h"
#include "default_config.h"
#include "Chassis_IRQ.h"

//namespace miro {

class Chassis {
public:

	Chassis(byte *PWM_pins, byte *DIR_pins);
#if defined(ENCODERS_ON)	
	Chassis(byte *PWM_pins, byte *DIR_pins, byte *ENCODER_pins);
#endif
	~Chassis();
	
	void Sync();

	/*Battery voltage measurement*/
	float getVoltage();
	
	/*
	Wheel rotation at a given PWM [-255..255] for a time in millis.
	*/
	int wheelRotatePWMTime(int *speedPWM, unsigned long time);
	
	/*
	Wheel rotation at a given PWM [-255..255].
	The return from the function will happen immediately.
	*/
	int wheelRotatePWM(int *speedPWM);
	
	/*Returns True if the wheel is spinning and False if it is not spinning*/
	bool wheelIsMoving(byte wheel) {return this->_wheel_move[wheel];}
	
	byte getWheelCount() { return WHEEL_COUNT; }
	
	//===================================================
	//===================================================
	//===================================================
#if defined(ENCODERS_ON)
	/*
	Calibration of motors. The calibration process determines the maximum and minimum motor speeds and break delays.
	During calibration, a table of the form "<wheel angular speed> - <motor voltage> - <break delay>" is entered into the EEPROM memory.
	Calibration must be done once on a new robot with a fully charged and balanced battery.
	During the calibration procedure, the robot motors rotate, so the robot must be mounted on a stand.
	The data obtained during calibration are used in many functions to set the initial values ​​of PWM motors,
	depending on the required angular speed of the wheel.
	*/
	void wheelCalibrate(byte wheel);
	
	float getCalibTableValue(byte wheel, byte parameter, byte record);

	/*
	Synchronous rotation of the wheels. IMPORTANT FUNCTION!
	speed [] - array-setting the angular speeds (deg / sec) for each wheel
	ang [] - array-setting the rotation angles of each wheel (degree)
	*/
	int wheelRotateAng(float *speed, float *ang, bool en_break);

	/*
	Wheel rotation at a given speed (without time or angle limit - until the wheels are clearly stopped).
	The return from the function will happen immediately. The wheel spin control process will be performed in the Sync () function.
	*/
	int wheelRotate(float *speed);

	/*Returns the odometry counter value of the selected motor (wheel)*/
	unsigned long wheelGetEncoder(byte wheel);

	/*Reset odometry counter value of the selected motor (wheel)*/
	void wheelResetEncoder(byte wheel);

	/*Returns the value of the angular velocity of the selected wheel (deg / sec)*/
	float wheelGetAngSpeed(byte wheel) { return (this->_wheelAngSpeed[wheel]); }

	/*Returns the value of the angular velocity of the selected wheel (rad / sec)*/
	float wheelGetAngSpeedRad(byte wheel) { return this->_wheelAngSpeed[wheel] / MIRO_PI2ANG; }

	/*Returns the length of the path traveled by the selected wheel (meters)*/
	float wheelGetPath(byte wheel);

	/*Returns the linear velocity of the wheel axis (m / s)*/
	float wheelGetLinSpeed(byte wheel) { return MIRO_PI * this->_wheelAngSpeed[wheel] * WHEEL_RADIUS / 180.0; }
	
#endif //ENCODERS_ON

private:

	float _vbat; //Battery volgage
	bool _wheel_move[WHEEL_COUNT];
	char _wheelDir[WHEEL_COUNT];

	byte _wheel_PWM_pins[WHEEL_COUNT];
	byte _wheel_DIR_pins[WHEEL_COUNT];
	
	void _init(byte *PWM_pins, byte *DIR_pins);
	
#if defined(ENCODERS_ON)
	
	byte _wheel_ENCODER_pins[WHEEL_COUNT];
	bool _wheel_sync_move;
	
	float _calib_wheel_table[WHEEL_COUNT][3][WHEEL_TABLE_SIZE];
	float _wheelAngSpeed[WHEEL_COUNT];

	float _wheelSetAng[WHEEL_COUNT];
	float _wheelSetAngSpeed[WHEEL_COUNT];

	unsigned long _wheelLastSync[WHEEL_COUNT];

//==========================================
	void _wheel_rotate(byte wheel);
	void _wheel_rotate_sync(bool en_break);
	
	int _eepromReadWheelTable(byte wheel, float *table);
	int _eepromWriteWheelTable(byte wheel, float *table);
	
	int _wheelGetU(float ang_speed, int wheel, float volts);
	int _wheelGetBDelay(float ang_speed, int wheel);
	
#endif //ENCODERS_ON

};

//} // end namespace