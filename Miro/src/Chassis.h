#ifndef chassis_h
#define chassis_h

#include "defs.h"
#include "config.h"
#include "default_config.h"
#include "Chassis_IRQ.h"

//namespace miro {

class Chassis {
public:

	Chassis() {};
	Chassis(uint8_t *PWM_pins, uint8_t *DIR_pins);
	void _init(uint8_t *PWM_pins, uint8_t *DIR_pins);
#if defined(ENCODERS_ON)	
	Chassis(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins);
	void _init(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins);
#endif // ENCODERS_ON
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
	bool wheelIsMoving(int8_t wheel) {return this->_wheel_move[wheel];}
	
	uint8_t getWheelCount() { return WHEEL_COUNT; }
	
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
	void wheelCalibrate(int8_t wheel);
	
	int getWheelTableValue(int8_t wheel, int8_t parameter, int8_t record);

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
	unsigned long wheelGetEncoder(int8_t wheel);

	/*Reset odometry counter value of the selected motor (wheel)*/
	void wheelResetEncoder(int8_t wheel);

	/*Returns the value of the angular velocity of the selected wheel (deg / sec)*/
	float wheelGetAngSpeed(int8_t wheel) { return (this->_wheelAngSpeed[wheel]); }

	/*Returns the value of the angular velocity of the selected wheel (rad / sec)*/
	float wheelGetAngSpeedRad(int8_t wheel) { return this->_wheelAngSpeed[wheel] / MIRO_PI2ANG; }

	/*Returns the length of the path traveled by the selected wheel (meters)*/
	float wheelGetPath(int8_t wheel);

	/*Returns the linear velocity of the wheel axis (m / s)*/
	float wheelGetLinSpeed(int8_t wheel) { return MIRO_PI * this->_wheelAngSpeed[wheel] * WHEEL_RADIUS / 180.0; }
	
#endif // ENCODERS_ON

private:

	float _vbat; //Battery volgage
	bool _wheel_move[WHEEL_COUNT];
	int8_t _wheelDir[WHEEL_COUNT];

	uint8_t _wheel_PWM_pins[WHEEL_COUNT];
	uint8_t _wheel_DIR_pins[WHEEL_COUNT];
	
#if defined(ENCODERS_ON)
	
	uint8_t _wheel_ENCODER_pins[WHEEL_COUNT];
	bool _wheel_sync_move;
	
	float _wheelAngSpeed[WHEEL_COUNT];

	float _wheelSetAng[WHEEL_COUNT];
	float _wheelSetAngSpeed[WHEEL_COUNT];

	unsigned long _wheelLastSync[WHEEL_COUNT];

//==========================================
	void _wheel_rotate_sync(bool en_break);
	
	int _eepromReadWheelTable(int8_t wheel, int *table);
	int _eepromWriteWheelTable(int8_t wheel, int *table);
	
	int _wheelGetU(float ang_speed, int wheel, float volts);
	int _wheelGetBDelay(float ang_speed, int wheel);
	
#endif // ENCODERS_ON

};

//} // end namespace

#endif