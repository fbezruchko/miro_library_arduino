#ifndef chassis_h
#define chassis_h

#include "chassis_config.h"

namespace miro {

#define MB_PI (3.14159265)
#define MB_PI2ANG (57.2957795)

class Chassis {
public:
	/*Init pins, modes, irq's - must call in setup()*/
	void Init();

    void printWheelTable();

	void Sync();

	/*Battery voltage measurement*/
	float getVoltage();

	/*
	Calibration of motors. The calibration process determines the maximum and minimum motor speeds and break delays.
	During calibration, a table of the form "<wheel angular speed> - <motor voltage> - <break delay>" is entered into the EEPROM memory.
	Calibration must be done once on a new robot with a fully charged and balanced battery.
	During the calibration procedure, the robot motors rotate, so the robot must be mounted on a stand.
	The data obtained during calibration are used in many functions to set the initial values ​​of PWM motors,
	depending on the required angular speed of the wheel.
	*/
	void wheelCalibrate(byte wheel);

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
	int wheelRotate(float *speed); //TO-DO

	/*Returns the odometry counter value of the selected motor (wheel)*/
	unsigned long wheelGetTachom(byte wheel);

	/*Reset odometry counter value of the selected motor (wheel)*/
	void wheelResetTachom(byte wheel);

	/*Returns the value of the angular velocity of the selected wheel (deg / sec)*/
	float wheelGetAngSpeed(byte wheel) { return (this->_wheelAngSpeed[wheel]); }

	/*Returns the value of the angular velocity of the selected wheel (rad / sec)*/
	float wheelGetAngSpeedRad(byte wheel) { return this->_wheelAngSpeed[wheel] / MB_PI2ANG; }

	/*Returns True if the wheel is spinning and False if it is not spinning*/
	bool wheelIsMoving(byte wheel) {return this->_wheel_move[wheel];}

	/*Returns the length of the path traveled by the selected wheel (meters)*/
	float wheelGetPath(byte wheel);

	/*Returns the linear velocity of the wheel axis (m / s)*/
	float wheelGetLinSpeed(byte wheel) { return MB_PI * this->_wheelAngSpeed[wheel] * WHEEL_RADIUS / 180.0; }
	
	byte getWheelCount() {return WHEEL_COUNT;}
	
	float getMaxLinSpeed();
	float getOptLinSpeed();
	float getMinLinSpeed();

	/*
	Rotation of ONE wheel.
	speed - set angular speed (deg / sec) for the wheel
	ang - setting the angle of rotation of the wheel (degrees)
	wheel - wheel number
	*/
	int wheelRotateAng_one(float speed, float ang, byte wheel);

private:
	void _wheel_rotate(byte wheel);
	void _wheel_rotate_sync(bool en_break);
	
	int _eepromReadWheelTable(byte wheel, float *table);
	int _eepromWriteWheelTable(byte wheel, float *table);
	
	int _wheelGetU(float ang_speed, int wheel, float volts);
	int _wheelGetBDelay(float ang_speed, int wheel);

	float _calib_wheel_table[WHEEL_COUNT][3][WHEEL_TABLE_SIZE];
	char _wheelDir[WHEEL_COUNT];
	float _wheelAngSpeed[WHEEL_COUNT];

	float _wheelSetAng[WHEEL_COUNT];
	float _wheelSetAngSpeed[WHEEL_COUNT];

	unsigned long _wheelLastSync[WHEEL_COUNT];

	float _vbat; //Battery volgage

	bool _wheel_sync_move;
	bool _wheel_move[WHEEL_COUNT];
};

} // end namespace

#endif
