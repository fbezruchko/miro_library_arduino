#pragma once

//namespace miro {
	
#if !defined(ENCODERS_ON)
	//#define ENCODERS_ON
#endif
	
#if !defined(ROBOT_MAX_DEVICES)
	#define ROBOT_MAX_DEVICES 12	//Максимальное число устройств, которые можно подключить к роботу.
#endif

#if !defined(PIN_VBAT)
	#define PIN_VBAT A0	//Пин для получения напряжения батареи (ячейки 1S - до 4.2В)
#endif

#if !defined(VOLTAGE_DIVIDER)
	#define VOLTAGE_DIVIDER 1
#endif

#if !defined(WHEEL_COUNT)
	#define WHEEL_COUNT 2	//Число ведущих колес у робота
#endif

#if !defined(ROBOT_DIAMETER)	
	#define ROBOT_DIAMETER (0.163) //ђасстояние между левым и правым колесом (метров)
#endif	
#if !defined(WHEEL_SEGMENTS)	
	#define WHEEL_SEGMENTS (30.0) //Число сегментов маркерной линейки тахометра. Надо переделать на 18, чтобы поворот робота на 90 градусов был точнее.
#endif	
#if !defined(WHEEL_RADIUS)	
	#define WHEEL_RADIUS (0.0345) //Радиус колеса (метров)
#endif
#if !defined(MEAN_DEPTH)
	#define MEAN_DEPTH 8 //МОЖНО НЕ ТРОГАТЬ! Глубина фильтра измерения скорости по тахометрам (размер массива для усреднения)
#endif	

/*Это оценочные значения, применяемые только при калибровке.*/
#if !defined(WHEEL_ANGLE_SPEED_MAX)
	#define WHEEL_ANGLE_SPEED_MAX (1500) //Максимальная угловая скорость вращения колес, которую могут развить двиагтели (град/сек). Выше этой сокрости колеса крутиться не могут
#endif
#if !defined(WHEEL_ANGLE_SPEED_MIN)
	#define WHEEL_ANGLE_SPEED_MIN (100) //Минимальная угловая скорость вращения колес (град/сек). Ниже этой сокрости колеса крутиться не могут
#endif	
#if !defined(MAX_WHEEL_BREAK_DELAY)
	#define MAX_WHEEL_BREAK_DELAY (60)
#endif

#if !defined(EEPROM_WHEEL_TABLE_ADDR)
	#define EEPROM_WHEEL_TABLE_ADDR 0
#endif
#if !defined(WHEEL_TABLE_SIZE)
	#define WHEEL_TABLE_SIZE (10)
#endif

//#define DEFAULT_PID_Kp (0.0192) //0.039 - 2,5с период колебаний
#if !defined (DEFAULT_PID_Kp)
	#define DEFAULT_PID_Kp (0.03)
#endif

//} // end namespace