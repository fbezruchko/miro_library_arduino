#include "SyncChassis.h"

#include "math.h"
#include "EEPROM.h"

#define SEGMENT_ANGLE (360.0/WHEEL_SEGMENTS)
#define WHEEL_TIME_MIN (360000000.0 / (WHEEL_ANGLE_SPEED_MAX * WHEEL_SEGMENTS))
#define WHEEL_TIME_MAX (360000000.0 / (WHEEL_ANGLE_SPEED_MIN * WHEEL_SEGMENTS))
#define WHEEL_TIME(speed) (360000000.0 / ((speed) * WHEEL_SEGMENTS))

#define VOLTS (0)
#define SPEED (1)
#define BREAKDELAY (2)

unsigned int dist_wheelTachomCount[WHEEL_COUNT];

float U[WHEEL_COUNT];
byte U0[WHEEL_COUNT];
bool dir[WHEEL_COUNT];
float Error[WHEEL_COUNT];

unsigned long wheel_time_max[WHEEL_COUNT];
unsigned long wheel_time_min[WHEEL_COUNT];

using namespace miro;

//++++++++++++++++++++++++++++++++++++++++++
//PUBLIC
//++++++++++++++++++++++++++++++++++++++++++

//========================================== Init

void SyncChassis::Init(byte *PWM_pins, byte *DIR_pins)
{
	pinMode(PIN_VBAT, INPUT);

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		wheel_PWM_pins[w] = PWM_pins[w];
		wheel_DIR_pins[w] = DIR_pins[w];

		pinMode(wheel_PWM_pins[w], OUTPUT);
		pinMode(wheel_DIR_pins[w], OUTPUT);
        digitalWrite(wheel_DIR_pins[w], LOW);
        analogWrite(wheel_PWM_pins[w], 0);
		this->_wheel_move[w] = false;
		this->_wheelDir[w] = 1;
	}
	this->_wheel_sync_move = false;
	
	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		pinMode(wheel_TACHOM_pins[w], INPUT);
		_wheelTachomCount[w] = 0;

		attachInterrupt(digitalPinToInterrupt(wheel_TACHOM_pins[w]), wheel_ISR[w], RISING);
		
		this->_eepromReadWheelTable(w, (float*)this->_calib_wheel_table[w]);
		wheel_time_max[w] = WHEEL_TIME(this->_calib_wheel_table[w][SPEED][0]);//Т. к. это можно посчитать заранее вне цикла - так и делаем
		wheel_time_min[w] = WHEEL_TIME(this->_calib_wheel_table[w][SPEED][WHEEL_TABLE_SIZE - 1]);
	}
}

//==========================================
//==========================================
//==========================================

//========================================== Sync

void SyncChassis::Sync()
{
	unsigned long wheelTime = 0;
	byte w;
	if (this->_wheel_sync_move)
	{
		for (w = 0; w < WHEEL_COUNT; w++)
		{
			if (this->_wheel_move[w])
			{
				if (_syncloop[w])
				{
					_syncloop[w] = false;
					wheelTime = 0;
					byte k = 0;
				
					for (byte i = 0; i < MEAN_DEPTH; i++)
					{
						if ((_wheelTimeArray[w][i] <= wheel_time_max[w]) && (_wheelTimeArray[w][i] >= wheel_time_min[w]))
						{
							wheelTime = wheelTime + _wheelTimeArray[w][i];
							k++;
						}
					}
					this->_wheelAngSpeed[w] = 0;
					if (k > 0)
					{
						wheelTime = wheelTime / k;
						this->_wheelAngSpeed[w] = (360000000.0/WHEEL_SEGMENTS) * ((float)this->_wheelDir[w] / wheelTime);
					}

					Error[w] = (this->_wheelSetAngSpeed[w] - this->_wheelAngSpeed[w]);
					U[w] = U[w] + DEFAULT_PID_Kp * Error[w];

					if (U[w] > 255) U[w] = 255;
					if (U[w] < -255) U[w] = -255;
					U0[w] = abs((int)U[w]);
					if (dir[w]) U0[w] = 255 - U0[w];
				}
			}
			else
			{
				U0[w] = 0;
				digitalWrite(wheel_DIR_pins[w], 0);
				digitalWrite(wheel_PWM_pins[w], 0);
			}
		}
	
		for (byte w = 0; w < WHEEL_COUNT; w++) analogWrite(wheel_PWM_pins[w], U0[w]);
	}

	return;
}

//========================================== printWheelTable

void SyncChassis::printWheelTable()
{	
    Serial.println(F("MOTORS CALIBRATION TABLE"));
	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		Serial.print(F("WHEEL "));
		Serial.println(w);
		Serial.print(F("VOLTS"));
		Serial.print(F("   "));
		Serial.print(F("DEG/SEC"));
		Serial.print(F("   "));
		Serial.println(F("BDELAYS"));

		for (int i = 0; i < WHEEL_TABLE_SIZE; i++)
		{
			Serial.print(this->_calib_wheel_table[w][VOLTS][i]);
			Serial.print(F("   "));
			Serial.print(this->_calib_wheel_table[w][SPEED][i]);
			Serial.print(F("   "));
			Serial.println(this->_calib_wheel_table[w][BREAKDELAY][i]);
		}
		Serial.println();
	}
    return;
}

//========================================== wheelCalibrate

void SyncChassis::wheelCalibrate(byte wheel)
{
	unsigned long wheelTime = 0;
	this->_vbat = this->getVoltage();

	digitalWrite(wheel_DIR_pins[wheel], LOW);
	digitalWrite(wheel_PWM_pins[wheel], LOW);

	this->_wheelAngSpeed[wheel] = 0;

	unsigned int U = 0;
	
	while ((this->_wheelAngSpeed[wheel] == 0) && (U <= 255))
	{
		U++;
		wheelTime = 0;
		
		analogWrite(wheel_PWM_pins[wheel], U);
		delay(WHEEL_TIME_MAX * 2 / 1000);
		
		for (int i = 0; i < MEAN_DEPTH; i++) wheelTime = wheelTime + _wheelTimeArray[wheel][i];
		if (wheelTime > 0)
		{
			wheelTime = wheelTime / MEAN_DEPTH;
			this->_wheelAngSpeed[wheel] = (360000000.0) / (WHEEL_SEGMENTS *  wheelTime);
		}
		else
		{
			this->_wheelAngSpeed[wheel] = 0;
		}
	}
	
	U = U + 10;
	byte cstep = (255 - U) / (WHEEL_TABLE_SIZE - 1);
	for (int i = 0; i < WHEEL_TABLE_SIZE; i++)
	{				
		if (U > 255) U = 255;
		wheelTime = 0;
		
		dist_wheelTachomCount[wheel] = _wheelTachomCount[wheel] + 2 * WHEEL_SEGMENTS;
		analogWrite(wheel_PWM_pins[wheel], U);
		while (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel]);
		if (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel])
		{
			i--;
			continue;
		}

		for (int m = 0; m < MEAN_DEPTH; m++) wheelTime = wheelTime + _wheelTimeArray[wheel][m];
		analogWrite(wheel_PWM_pins[wheel], 0);
		delay(500);
		
		if (wheelTime > 0)
		{
			wheelTime = wheelTime / MEAN_DEPTH;
			this->_wheelAngSpeed[wheel] = (360000000.0) / (WHEEL_SEGMENTS *  wheelTime);
		}
		else
		{
			this->_wheelAngSpeed[wheel] = 0;
		}
		
		int error_count;
		int min_error_count = WHEEL_SEGMENTS;
		byte wheelBreakDelay = MAX_WHEEL_BREAK_DELAY;
		byte opt1_wheelBreakDelay = MAX_WHEEL_BREAK_DELAY;
		
		while (wheelBreakDelay > 0)
		{
			error_count = 0;
			dist_wheelTachomCount[wheel] = _wheelTachomCount[wheel] + WHEEL_SEGMENTS;
			analogWrite(wheel_PWM_pins[wheel], U);
			while (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel]);
			digitalWrite(wheel_DIR_pins[wheel], !dir[wheel]);
			digitalWrite(wheel_PWM_pins[wheel], dir[wheel]);
			delay(wheelBreakDelay);//Задержка для жесктого останова - именно на это время на двигатели подается обратное напряжение
			digitalWrite(wheel_DIR_pins[wheel], dir[wheel]);
			delay(500);
			
			if (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel]) continue;
			
			error_count = _wheelTachomCount[wheel] - dist_wheelTachomCount[wheel];
			if (min_error_count > error_count)
			{
				min_error_count = error_count;
				opt1_wheelBreakDelay = wheelBreakDelay;
			}
			wheelBreakDelay = wheelBreakDelay - MAX_WHEEL_BREAK_DELAY/5;
		}
		
		min_error_count = WHEEL_SEGMENTS;
		byte opt2_wheelBreakDelay = opt1_wheelBreakDelay + MAX_WHEEL_BREAK_DELAY/5;
		for (wheelBreakDelay = opt2_wheelBreakDelay; wheelBreakDelay > opt1_wheelBreakDelay - MAX_WHEEL_BREAK_DELAY/10; wheelBreakDelay = wheelBreakDelay - 2)
		{
			error_count = 0;                
			for (int t = 0; t < 5; t++)
			{
				dist_wheelTachomCount[wheel] = _wheelTachomCount[wheel] + WHEEL_SEGMENTS;
				analogWrite(wheel_PWM_pins[wheel], U);
				while (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel]);
				digitalWrite(wheel_DIR_pins[wheel], !dir[wheel]);
				digitalWrite(wheel_PWM_pins[wheel], dir[wheel]);
				delay(wheelBreakDelay);//Задержка для жесктого останова - именно на это время на двигатели подается обратное напряжение
				digitalWrite(wheel_DIR_pins[wheel], dir[wheel]);
				delay(500);
				
				if (dist_wheelTachomCount[wheel] > _wheelTachomCount[wheel])
				{
					t--;
					continue;
				}
				error_count = error_count + (_wheelTachomCount[wheel] - dist_wheelTachomCount[wheel]);
			}
			if (min_error_count > error_count)
			{
				min_error_count = error_count;
				opt2_wheelBreakDelay = wheelBreakDelay;
			}	
		}
		
		this->_calib_wheel_table[wheel][VOLTS][i] = this->_vbat * (float)U / 255.0;
		this->_calib_wheel_table[wheel][SPEED][i] = this->_wheelAngSpeed[wheel];
		this->_calib_wheel_table[wheel][BREAKDELAY][i] = opt2_wheelBreakDelay;
		if (i < (WHEEL_TABLE_SIZE - 2)) U += cstep;
		else U = 255;
		
	}
	analogWrite(wheel_PWM_pins[wheel], 0);
	
	this->_eepromWriteWheelTable(wheel, (float*)this->_calib_wheel_table[wheel]);
	
	return;
}

//========================================== wheelRotateAng

int SyncChassis::wheelRotateAng(float *speed, float *ang, bool en_break)
{
	byte w;
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if ((fabs(speed[w]) < this->_calib_wheel_table[w][SPEED][0]) && (fabs(speed[w]) > 0)) return -1; //ошибка - невозможно совершить поворот колеса с такой маленькой скоростью
		if ((fabs(ang[w]) < float(SEGMENT_ANGLE)) && (fabs(speed[w]) > 0)) return -2; //ошибка - невозможно совершить поворот колеса на такой маленький угол
	}

	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if (speed[w] != 0) break;
	}
	
	if (w == WHEEL_COUNT) //Остановка моторов
	{
		for (w = 0; w < WHEEL_COUNT; w++)
		{
			analogWrite(wheel_PWM_pins[w], 0);
			digitalWrite(wheel_DIR_pins[w], LOW);
			this->_wheelSetAng[w] = 0;
			this->_wheelSetAngSpeed[w] = 0;
			this->_wheel_move[w] = false;
		}
		this->_wheel_sync_move = false;
		return 0;
	}
	else
	{
		for (byte w = 0; w < WHEEL_COUNT; w++)
		{
			this->_wheelSetAng[w] = ang[w];
			this->_wheelSetAngSpeed[w] = speed[w];
		}
		this->_wheel_sync_move = true;
	}

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		this->_wheelDir[w] = 1;
		if ((this->_wheelSetAng[w] * this->_wheelSetAngSpeed[w]) < 0) this->_wheelDir[w] = -1;

		dist_wheelTachomCount[w] = _wheelTachomCount[w] + int(0.51 + (fabs(this->_wheelSetAng[w]) / float(SEGMENT_ANGLE))); //число сегментов маркерной линейки колеса, которое нужно отсчитать, чтобы повернуть на требуемый угол;
		this->_vbat = getVoltage();
		U[w] = this->_wheelGetU(fabs(this->_wheelSetAngSpeed[w]), w, this->_vbat);
		if (U[w] < 0) return -1;
		U[w] = (float)this->_wheelDir[w] * U[w];
	}

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		memset(_wheelTimeArray[w], 0, sizeof(long) * MEAN_DEPTH);

		dir[w] = false;
		if (this->_wheelDir[w] < 0) dir[w] = true;

		this->_wheelSetAngSpeed[w] = (float)this->_wheelDir[w] * fabs(this->_wheelSetAngSpeed[w]);

		digitalWrite(wheel_DIR_pins[w], dir[w]);
		digitalWrite(wheel_PWM_pins[w], dir[w]);
		this->_wheel_move[w] = true;
	}

	this->_wheel_rotate_sync(en_break);

	for (byte w = 0; w < WHEEL_COUNT; w++) this->_wheel_move[w] = false;

	return 0;
}

//========================================== wheelRotate

int SyncChassis::wheelRotate(float *speed)
{
	byte w;
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if ((fabs(speed[w]) < this->_calib_wheel_table[w][SPEED][0]) && (fabs(speed[w]) > 0)) return -1; //ошибка - невозможно совершить поворот колеса с такой маленькой скоростью
	}

	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if (speed[w] != 0) break;
	}
	
	if (w == WHEEL_COUNT) //Остановка моторов
	{
		for (w = 0; w < WHEEL_COUNT; w++)
		{
			analogWrite(wheel_PWM_pins[w], 0);
			digitalWrite(wheel_DIR_pins[w], LOW);
			this->_wheelSetAngSpeed[w] = 0;
			this->_wheel_move[w] = false;
		}
		this->_wheel_sync_move = false;
		return 0;
	}
	else
	{
		for (byte w = 0; w < WHEEL_COUNT; w++)
		{
			this->_wheelSetAngSpeed[w] = speed[w];
		}
		this->_wheel_sync_move = true;
	}

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		this->_wheelDir[w] = 1;
		if (this->_wheelSetAngSpeed[w] < 0) this->_wheelDir[w] = -1;

		this->_vbat = getVoltage();
		U[w] = this->_wheelGetU(fabs(this->_wheelSetAngSpeed[w]), w, this->_vbat);
		if (U[w] < 0) return -1;
		U[w] = (float)this->_wheelDir[w] * U[w];
	}

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		dir[w] = false;
		if (this->_wheelDir[w] < 0) dir[w] = true;

		this->_wheelSetAngSpeed[w] = (float)this->_wheelDir[w] * fabs(this->_wheelSetAngSpeed[w]);

		digitalWrite(wheel_DIR_pins[w], dir[w]);
		digitalWrite(wheel_PWM_pins[w], dir[w]);
		this->_wheel_move[w] = true;
	}

	return 0;
}

//========================================== wheelGetPath
float SyncChassis::wheelGetPath(byte wheel) { return 2 * MB_PI * WHEEL_RADIUS * ((float)_wheelTachomCount[wheel] / WHEEL_SEGMENTS); }

//========================================== wheelGetTachom
unsigned long SyncChassis::wheelGetTachom(byte wheel) { return _wheelTachomCount[wheel]; }

//========================================== wheelResetTachom
void SyncChassis::wheelResetTachom(byte wheel) { _wheelTachomCount[wheel] = 0; }

//========================================== getMaxLinSpeed
float SyncChassis::getMaxLinSpeed()
{
	unsigned long min_wheel_speed_max = this->_calib_wheel_table[0][SPEED][WHEEL_TABLE_SIZE - 1];
	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->_calib_wheel_table[w][SPEED][WHEEL_TABLE_SIZE - 1] < min_wheel_speed_max) min_wheel_speed_max = this->_calib_wheel_table[w][SPEED][WHEEL_TABLE_SIZE - 1];
	}
	return (min_wheel_speed_max / 180.0) * MB_PI * WHEEL_RADIUS;
}

//========================================== getMinLinSpeed
float SyncChassis::getMinLinSpeed()
{
	unsigned long min_wheel_speed_min = this->_calib_wheel_table[0][SPEED][0];
	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->_calib_wheel_table[w][SPEED][0] < min_wheel_speed_min) min_wheel_speed_min = this->_calib_wheel_table[w][SPEED][0];
	}
	return (min_wheel_speed_min / 180.0) * MB_PI * WHEEL_RADIUS;
}

//========================================== getOptLinSpeed
float SyncChassis::getOptLinSpeed()
{
	unsigned long opt_wheel_speed_min = this->_calib_wheel_table[0][SPEED][WHEEL_TABLE_SIZE/2];
	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		if (this->_calib_wheel_table[w][SPEED][WHEEL_TABLE_SIZE/2] < opt_wheel_speed_min) opt_wheel_speed_min = this->_calib_wheel_table[w][SPEED][WHEEL_TABLE_SIZE/2];
	}
	return (opt_wheel_speed_min / 180.0) * MB_PI * WHEEL_RADIUS;
}

//++++++++++++++++++++++++++++++++++++++++++
//PRIVATE
//++++++++++++++++++++++++++++++++++++++++++

int SyncChassis::_eepromWriteWheelTable(byte wheel, float *table)
{
	unsigned int N = WHEEL_TABLE_SIZE*(3 * sizeof(float));
	unsigned int addr = EEPROM_WHEEL_TABLE_ADDR + wheel*WHEEL_TABLE_SIZE*(3 * sizeof(float));
	byte *p = (byte*)table;

	for (unsigned int i = 0; i < N; i++)
	{
		EEPROM.write(addr + i, *(p + i));
	}
	return 0;
}


int SyncChassis::_eepromReadWheelTable(byte wheel, float *table)
{
	unsigned int N = WHEEL_TABLE_SIZE*(3 * sizeof(float));
	unsigned int addr = EEPROM_WHEEL_TABLE_ADDR + wheel*WHEEL_TABLE_SIZE*(3 * sizeof(float));
	byte *p = (byte*)table;

	for (unsigned int i = 0; i < N; i++)
	{
		*(p + i) = EEPROM.read(addr + i);
	}

	return 0;
}

int SyncChassis::_wheelGetU(float ang_speed, int wheel, float volts)
{
	unsigned int U = 0;
	float v = 0;
	byte i = 0;

	if (volts == 0) return -3;

	if ((ang_speed < this->_calib_wheel_table[wheel][SPEED][0]) || (ang_speed > this->_calib_wheel_table[wheel][SPEED][WHEEL_TABLE_SIZE - 1])) return -1;

	for (i = 0; i < WHEEL_TABLE_SIZE; i++)
	{
		if (this->_calib_wheel_table[wheel][SPEED][i] >= ang_speed) break;
	}

	//(v-v1)/(v2-v1) = (s-s1)/(s2-s1) - уравнение прямой через две точки, s - SPEED (град/сек), v - VOLTS (В)
	//откуда v = ((s-s1)*(v2-v1)/(s2-s1)) + v1

	v = this->_calib_wheel_table[wheel][VOLTS][i - 1] + ((ang_speed - this->_calib_wheel_table[wheel][SPEED][i - 1])*(this->_calib_wheel_table[wheel][VOLTS][i] - this->_calib_wheel_table[wheel][VOLTS][i - 1]) / (this->_calib_wheel_table[wheel][SPEED][i] - this->_calib_wheel_table[wheel][SPEED][i - 1]));
	U = (255 * v) / volts;

	if (U > 255) U = 255;

	return U;
}

int SyncChassis::_wheelGetBDelay(float ang_speed, int wheel)
{
	unsigned int BDelay = 0;
	byte i = 0;

	if ((ang_speed < this->_calib_wheel_table[wheel][SPEED][0]) || (ang_speed > this->_calib_wheel_table[wheel][SPEED][WHEEL_TABLE_SIZE - 1])) return -1;

	for (i = 0; i < WHEEL_TABLE_SIZE; i++)
	{
		if (this->_calib_wheel_table[wheel][SPEED][i] >= ang_speed) break;
	}

	BDelay = this->_calib_wheel_table[wheel][BREAKDELAY][i];

	return BDelay;
}

void SyncChassis::_wheel_rotate(byte wheel)
{
	unsigned long wheelTime = 0;

	while (1)
	{
		if (dist_wheelTachomCount[wheel] <= _wheelTachomCount[wheel]) break; //Выход из цикла при достижении необходимого числа отметок маркера - поворот совершен
		
		if (_syncloop[wheel])
		{
			_syncloop[wheel] = false;
			wheelTime = 0;
			byte k = 0;
			for (byte i = 0; i < MEAN_DEPTH; i++)
			{
				if ((_wheelTimeArray[wheel][i] <= wheel_time_max[wheel]) && (_wheelTimeArray[wheel][i] >= wheel_time_min[wheel]))
				{
					wheelTime = wheelTime + _wheelTimeArray[wheel][i];
					k++;
				}
			}
			this->_wheelAngSpeed[wheel] = 0;
			if (k > 0)
			{
				wheelTime = wheelTime / k;
				this->_wheelAngSpeed[wheel] = (360000000.0/WHEEL_SEGMENTS) * ((float)this->_wheelDir[wheel] / wheelTime);
			}

			Error[wheel] = (this->_wheelSetAngSpeed[wheel] - this->_wheelAngSpeed[wheel]);
			U[wheel] = U[wheel] + DEFAULT_PID_Kp * Error[wheel];
		}

		if (U[wheel] > 255) U[wheel] = 255;
		if (U[wheel] < -255) U[wheel] = -255;
		U0[wheel] = abs((int)U[wheel]);
		if (dir[wheel]) U0[wheel] = 255 - U0[wheel];

		analogWrite(wheel_PWM_pins[wheel], U0[wheel]);
	}

	digitalWrite(wheel_DIR_pins[wheel], !dir[wheel]);
	digitalWrite(wheel_PWM_pins[wheel], dir[wheel]);
	
	delay(this->_wheelGetBDelay(fabs(this->_wheelAngSpeed[wheel]), wheel));//Задержка для жесктого останова - именно на это время на двигатели подается обратное напряжение
	digitalWrite(wheel_DIR_pins[wheel], dir[wheel]);

	dist_wheelTachomCount[wheel] = _wheelTachomCount[wheel];
	this->_wheelAngSpeed[wheel] = 0;
	this->_wheelSetAngSpeed[wheel] = 0;
}

void SyncChassis::_wheel_rotate_sync(bool en_break)
{
	unsigned long wheelTime = 0;
	
	while (1)
	{
		byte w;
		for (w = 0; w < WHEEL_COUNT; w++)
		{
			if (dist_wheelTachomCount[w] <= _wheelTachomCount[w]) break; //Выход из цикла при достижении необходимого числа отметок маркера - поворот совершен
		}
		if (w < WHEEL_COUNT) break; //Тот break что выше выйдет только из цикла for, а этой строчкой мы выходим из всего цикла while
		//При этом, выход из цикла произойдет когда хотя бы одно из колес совершит поворот.

		for (w = 0; w < WHEEL_COUNT; w++)
		{
			if (_syncloop[w])
			{
				_syncloop[w] = false;
				wheelTime = 0;
				byte k = 0;
				
				for (byte i = 0; i < MEAN_DEPTH; i++)
				{
					if ((_wheelTimeArray[w][i] <= wheel_time_max[w]) && (_wheelTimeArray[w][i] >= wheel_time_min[w]))
					{
						wheelTime = wheelTime + _wheelTimeArray[w][i];
						k++;
					}
				}
				this->_wheelAngSpeed[w] = 0;
				if (k > 0)
				{
					wheelTime = wheelTime / k;
					this->_wheelAngSpeed[w] = (360000000.0/WHEEL_SEGMENTS) * ((float)this->_wheelDir[w] / wheelTime);
				}

				Error[w] = (this->_wheelSetAngSpeed[w] - this->_wheelAngSpeed[w]);
				U[w] = U[w] + DEFAULT_PID_Kp * Error[w];

				if (U[w] > 255) U[w] = 255;
				if (U[w] < -255) U[w] = -255;
				U0[w] = abs((int)U[w]);
				if (dir[w]) U0[w] = 255 - U0[w];
			}
		}
		for (byte w = 0; w < WHEEL_COUNT; w++) analogWrite(wheel_PWM_pins[w], U0[w]);
	}
	if (en_break)
	{
		for (byte w = 0; w < WHEEL_COUNT; w++)
		{
			digitalWrite(wheel_DIR_pins[w], !dir[w]);
			digitalWrite(wheel_PWM_pins[w], dir[w]);
		}
		delay(this->_wheelGetBDelay(fabs(this->_wheelAngSpeed[0]), 0));//Задержка для жесктого останова - именно на это время на двигатели подается обратное напряжение
	}
	for (byte w = 0; w < WHEEL_COUNT; w++) 
	{
		digitalWrite(wheel_DIR_pins[w], dir[w]);
		digitalWrite(wheel_PWM_pins[w], dir[w]);
	}

	for (byte w = 0; w < WHEEL_COUNT; w++)
	{
		dist_wheelTachomCount[w] = _wheelTachomCount[w];
		this->_wheelAngSpeed[w] = 0;
		this->_wheelSetAngSpeed[w] = 0;
	}
}
