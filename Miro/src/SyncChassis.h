#ifndef sync_chassis_h
#define sync_chassis_h

#include "chassis_config.h"
#include "BasicChassis.h"

namespace miro {
	
#if !defined (CHASSIS_W_ENCODERS)
	#define MB_PI (3.14159265)
	#define MB_PI2ANG (57.2957795)
	
	#define ROBOT_DIAMETER (0.163) //ђасстояние между левым и правым колесом (метров)
	#define WHEEL_SEGMENTS (30.0) //Число сегментов маркерной линейки тахометра. Надо переделать на 18, чтобы поворот робота на 90 градусов был точнее.
	#define WHEEL_RADIUS (0.0345) //Радиус колеса (метров)
	#define MEAN_DEPTH 8 //МОЖНО НЕ ТРОГАТЬ! Глубина фильтра измерения скорости по тахометрам (размер массива для усреднения)

	/*Это оценочные значения, применяемые только при калибровке.*/
	#define WHEEL_ANGLE_SPEED_MAX (1500) //Максимальная угловая скорость вращения колес, которую могут развить двиагтели (град/сек). Выше этой сокрости колеса крутиться не могут
	#define WHEEL_ANGLE_SPEED_MIN (100) //Минимальная угловая скорость вращения колес (град/сек). Ниже этой сокрости колеса крутиться не могут
	#define MAX_WHEEL_BREAK_DELAY (60)

	#define EEPROM_WHEEL_TABLE_ADDR 0
	#define WHEEL_TABLE_SIZE (10)

	//#define DEFAULT_PID_Kp (0.0192) //0.039 - 2,5с период колебаний
	#define DEFAULT_PID_Kp (0.03)
	
	static byte wheel_TACHOM_pins[WHEEL_COUNT] = { 2, 3 };
#endif

static volatile unsigned long _wheelTimeArray[WHEEL_COUNT][MEAN_DEPTH];
static volatile long _wheelTime[WHEEL_COUNT];
static volatile long _wheelTime_last[WHEEL_COUNT];
static volatile unsigned int _wheelTachomCount[WHEEL_COUNT];
static volatile bool _syncloop[WHEEL_COUNT] = { true, true };

static void wheelISR0()
{
	cli();
	_wheelTime[0] = micros();
	_wheelTimeArray[0][_wheelTachomCount[0] % MEAN_DEPTH] = _wheelTime[0] - _wheelTime_last[0];
	_wheelTime_last[0] = _wheelTime[0];
	_wheelTachomCount[0]++;
	_syncloop[0] = true;
	sei();
}

static void wheelISR1()
{
	cli();
	_wheelTime[1] = micros();
	_wheelTimeArray[1][_wheelTachomCount[1] % MEAN_DEPTH] = _wheelTime[1] - _wheelTime_last[1];
	_wheelTime_last[1] = _wheelTime[1];
	_wheelTachomCount[1]++;
	_syncloop[1] = true;
	sei();
}

static void(*wheel_ISR[WHEEL_COUNT])() = { wheelISR0, wheelISR1 };

class SyncChassis : public BasicChassis {
public:

	void Init(byte *PWM_pins, byte *DIR_pins);
	
	void Sync();
	
	void printWheelTable();
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
	int wheelRotate(float *speed);

	/*Returns the odometry counter value of the selected motor (wheel)*/
	unsigned long wheelGetTachom(byte wheel);

	/*Reset odometry counter value of the selected motor (wheel)*/
	void wheelResetTachom(byte wheel);

	/*Returns the value of the angular velocity of the selected wheel (deg / sec)*/
	float wheelGetAngSpeed(byte wheel) { return (this->_wheelAngSpeed[wheel]); }

	/*Returns the value of the angular velocity of the selected wheel (rad / sec)*/
	float wheelGetAngSpeedRad(byte wheel) { return this->_wheelAngSpeed[wheel] / MB_PI2ANG; }

	/*Returns the length of the path traveled by the selected wheel (meters)*/
	float wheelGetPath(byte wheel);

	/*Returns the linear velocity of the wheel axis (m / s)*/
	float wheelGetLinSpeed(byte wheel) { return MB_PI * this->_wheelAngSpeed[wheel] * WHEEL_RADIUS / 180.0; }
	
	float getMaxLinSpeed();
	float getOptLinSpeed();
	float getMinLinSpeed();

private:

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
};

} // end namespace

#endif
