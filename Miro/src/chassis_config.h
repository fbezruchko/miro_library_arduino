#ifndef chassis_config_h
#define chassis_config_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

namespace miro {
	
#define CHASSIS_W_ENCODERS 1

#define PIN_VBAT A0	//Пин для получения напряжения батареи (ячейки 1S - до 4.2В)
#define VOLTAGE_DIVIDER 1

#define WHEEL_COUNT 2	//Число ведущих колес у робота

#if defined (CHASSIS_W_ENCODERS)
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

//static byte wheel_PWM_pins[WHEEL_COUNT] = { 5, 6 };
//static byte wheel_DIR_pins[WHEEL_COUNT] = { 4, 7 };

} // end namespace

#endif
