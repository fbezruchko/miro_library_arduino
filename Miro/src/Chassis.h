#ifndef chassis_h
#define chassis_h

#include "chassis_config.h"

class Chassis {
public:
	/*Инициализация (назначение пинов, их режимов, прерываний и пр. - функция должна вызываться в setup()*/
	void Init();

    void printWheelTable();

	void Sync();

	/*Измерить и вычислить напряжеине батареи*/
	float getVoltage();

	/*Калибровка двигателей. В процессе калибровки определяется максимальная и минимальная скорость вращения двигателей.
	При калибровке в память EEPROM заносится таблица вида "<угловая скорость колеса> - <напряжение на моторе>".
	Калибровку необходимо проводить один раз на новом роботе с полностью заряженной и отбаллансированной батареей.
	Во вроемя процедуры калибровки двигатели робота вращаются, поэтому робот необходимо закрепить на подставке.
	Данные полученные в ходе калибровки используются во многих функциях для установки начальных значений ШИМ двигателей,
	в зависимости от требуемой угловой скосроти колеса.
	*/
	void wheelCalibrate();

	/*Сихронное вращение колес. ВАЖНЕЙШАЯ ФУНКЦИЯ!
	speed[] - массив-заданние угловых скоростей (град/сек) для каждого колеса
	ang[] - массив-задание углов поворота каждого колеса (град)
	*/
	int wheelRotateAng(float *speed, float *ang);

	/*Вращение колес заданной скоростью (без ограничения по времени или углу - пока колеса не будут явно остановлены).
	Возврат из функции произойдет незамедлительно. Процесс управления вращеинем колеса будет выполняться в функции Sync().
	*/
	int wheelRotate(float *speed); //Нормально не реализовано

	/*Возвращает значение счетчика тахометра выбранного мотор-колеса*/
	unsigned long wheelGetTachom(byte wheel);

	/*Сброс значения счетчика тахометра выбранного мотор - колеса*/
	void wheelResetTachom(byte wheel);

	/*Вовзращает значение угловой скорости выбранного колеса (град/сек)*/
	float wheelGetAngSpeed(byte wheel);

	/*Вовзращает значение угловой скорости выбранного колеса (рад/сек)*/
	float wheelGetAngSpeedRad(byte wheel);

	/*Возвращает True если колесо вращается и False если не вращается*/
	bool wheelIsMoving(byte wheel);

	/*Возвращает длину пути, пройденного выбранным колесом (метров)*/
	float wheelGetPath(byte wheel);

	/*Возвращает линейную скорость оси колеса (м/с)*/
	float wheelGetLinSpeed(byte wheel);

	/*Вращение ОДНОГО колеса.
	speed - заданние угловой скорости (град/сек) для колеса
	ang - задание угла поворота колеса (град)
	wheel - номер колеса
	*/
	int wheelRotateAng_one(float speed, float ang, byte wheel);

private:
	void _wheel_rotate(byte wheel);
	void _wheel_rotate_sync();
	int _eepromReadWheelTable(float *table);
	int _eepromWriteWheelTable(float *table);
	int _wheelGetU(float ang_speed, int wheel, float volts);

	float _calib_wheel_table[WHEEL_COUNT][3
][WHEEL_TABLE_SIZE];
	char _wheelDir[WHEEL_COUNT];
	float _wheelAngSpeed[WHEEL_COUNT];

	float _wheelSetAng[WHEEL_COUNT];
	float _wheelSetAngSpeed[WHEEL_COUNT];

	unsigned long _wheelLastSync[WHEEL_COUNT];

	float _vbat; //Напряжение батареи робота

	bool _wheel_sync_move;
	bool _wheel_move[WHEEL_COUNT];
};

#endif
