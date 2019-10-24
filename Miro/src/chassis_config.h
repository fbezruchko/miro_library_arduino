#ifndef chassis_config_h
#define chassis_config_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PIN_VBAT A0	//Пин для получения напряжения батареи (ячейки 1S - до 4.2В)
#define VOLTAGE_DIVIDER 1

#define WHEEL_COUNT 2	//Число ведущих колес у робота

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

namespace miro {

//static byte wheel_PWM_pins[WHEEL_COUNT] = { 5, 6 };
//wheel_PWM_pins[0] = 5;
//wheel_PWM_pins[1] = 6;

//static byte wheel_DIR_pins[WHEEL_COUNT] = { 4, 7 };
//wheel_DIR_pins[0] = 4;
//wheel_DIR_pins[1] = 7;

static byte wheel_TACHOM_pins[WHEEL_COUNT] = { 2, 3 };
//wheel_TACHOM_pins[0] = 2; //Пин тахометра колеса 0 (этот пин поддерживает аппаратные прерывания)
//wheel_TACHOM_pins[1] = 3; //Пин тахометра колеса 1 (этот пин поддерживает аппаратные прерывания)

//#############################################################################################################
//#############################################################################################################

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

} // end namespace

#endif
