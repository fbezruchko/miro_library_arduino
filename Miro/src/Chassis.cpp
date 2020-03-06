#include "Arduino.h"

#include "Chassis.h"
#include "defs.h"

#include "math.h"
#include "EEPROM.h"
 
//++++++++++++++++++++++++++++++++++++++++++
//PUBLIC
//++++++++++++++++++++++++++++++++++++++++++
#if defined(ENCODERS_ON)
//namespace miro {
	unsigned long wheel_time_max[WHEEL_COUNT];
	unsigned long wheel_time_min[WHEEL_COUNT];

	unsigned int dist_wheelEncoderCount[WHEEL_COUNT];

	float U[WHEEL_COUNT];
	uint8_t U0[WHEEL_COUNT];
	bool dir[WHEEL_COUNT];
	float Error[WHEEL_COUNT];
	float Error1[WHEEL_COUNT];
#if defined(DEBUG_WHEEL_PID)
	float debug_wheel_pid[DEBUG_WHEEL_PID_I_DEPTH];
	float debug_wheel_pid_max_error = 0;
	int pid_i = 0;
#endif
//}
#endif  // ENCODERS_ON

//using namespace miro;

//========================================== C
Chassis::Chassis(uint8_t *PWM_pins, uint8_t *DIR_pins)
{
	this->_init(PWM_pins, DIR_pins);
}

#if defined(ENCODERS_ON)
Chassis::Chassis(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins)
{
	this->_init(PWM_pins, DIR_pins, ENCODER_pins);
}
#endif  // ENCODERS_ON

void Chassis::_init(uint8_t *PWM_pins, uint8_t *DIR_pins)
{
	pinMode(PIN_VBAT, INPUT);

	for (int8_t w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheel_PWM_pins[w] = PWM_pins[w];
		this->_wheel_DIR_pins[w] = DIR_pins[w];

		pinMode(this->_wheel_PWM_pins[w], OUTPUT);
		pinMode(this->_wheel_DIR_pins[w], OUTPUT);
        digitalWrite(this->_wheel_DIR_pins[w], LOW);
        analogWrite(this->_wheel_PWM_pins[w], 0);
		this->_wheel_move[w] = false;
		this->_wheelDir[w] = 1;
	}
}

#if defined(ENCODERS_ON)
void Chassis::_init(uint8_t *PWM_pins, uint8_t *DIR_pins, uint8_t *ENCODER_pins)
{
	pinMode(PIN_VBAT, INPUT);

	for (int8_t w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheel_PWM_pins[w] = PWM_pins[w];
		this->_wheel_DIR_pins[w] = DIR_pins[w];

		pinMode(this->_wheel_PWM_pins[w], OUTPUT);
		pinMode(this->_wheel_DIR_pins[w], OUTPUT);
        digitalWrite(this->_wheel_DIR_pins[w], LOW);
        analogWrite(this->_wheel_PWM_pins[w], 0);
		this->_wheel_move[w] = false;
		this->_wheelDir[w] = 1;
	}
	
	for (int8_t w = WHEEL_COUNT-1; w >= 0; --w)
	{
		_wheel_ENCODER_pins[w] = ENCODER_pins[w];
		pinMode(_wheel_ENCODER_pins[w], INPUT);
		_IRQ_wheelEncoderCount[w] = 0;

		attachInterrupt(digitalPinToInterrupt(this->_wheel_ENCODER_pins[w]), wheel_ISR[w], RISING);
		
		wheel_time_max[w] = WHEEL_TIME(this->getWheelTableValue(w, SPEED, 0));//Т. к. это можно посчитать заранее вне цикла - так и делаем
		wheel_time_min[w] = WHEEL_TIME(this->getWheelTableValue(w, SPEED, WHEEL_TABLE_SIZE - 1));
	}
	this->_wheel_sync_move = false;
}
#endif

//========================================== D
Chassis::~Chassis()
{
}

//========================================== Sync

void Chassis::Sync()
{
#if defined(ENCODERS_ON)
	
	unsigned long wheelTime = 0;
	int8_t w;
	
	if (this->_wheel_sync_move)
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			if (this->_wheel_move[w])
			{
				if (_IRQ_syncloop[w])
				{
					_IRQ_syncloop[w] = false;
					wheelTime = 0;
					int8_t k = 0;
				
					for (int8_t i = MEAN_DEPTH-1; i >= 0; --i)
					{
						if ((_IRQ_wheelTimeArray[w][i] <= wheel_time_max[w]) && (_IRQ_wheelTimeArray[w][i] >= wheel_time_min[w]))
						{
							wheelTime = wheelTime + _IRQ_wheelTimeArray[w][i];
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
					U[w] = U[w] + DEFAULT_PID_Kd * (Error[w] - Error1[w]);
					Error1[w] = Error[w];
					if (U[w] > 255) U[w] = 255;
					if (U[w] < -255) U[w] = -255;
					U0[w] = abs((int)U[w]);
					if (dir[w]) U0[w] = 255 - U0[w];
				}
			}
			else
			{
				U0[w] = 0;
				digitalWrite(_wheel_DIR_pins[w], 0);
				digitalWrite(_wheel_PWM_pins[w], 0);
			}
		}
	
		for (w = WHEEL_COUNT-1; w >= 0; --w) analogWrite(_wheel_PWM_pins[w], U0[w]);
	}

#endif  // ENCODERS_ON

	return;
}

//========================================== GetVoltage

float Chassis::getVoltage()
{
	float v = 0;
	for (char i = 10-1; i >= 0; --i)
	{
		v = v + analogRead(PIN_VBAT);
	}
	v = VOLTAGE_DIVIDER * 5.0 * ((v / 10) / 1023.0);

	return v;
}

//========================================== wheelRotatePWMTime

int Chassis::wheelRotatePWMTime(int *speedPWM, unsigned long time)
{
	int8_t w;	
	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheel_move[w] = true;
		if (speedPWM[w] < 0) 
		{
			digitalWrite(_wheel_DIR_pins[w], HIGH);
			analogWrite(_wheel_PWM_pins[w], 255 + (speedPWM[w]%255));
		}
		else
		{
			digitalWrite(_wheel_DIR_pins[w], LOW);
			analogWrite(_wheel_PWM_pins[w], speedPWM[w]%255);
		}
	}
	delay(time);
	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		digitalWrite(_wheel_DIR_pins[w], LOW);
		analogWrite(_wheel_PWM_pins[w], 0);
		this->_wheel_move[w] = false;
	}
	
	return 0;
}

//========================================== wheelRotatePWM

int Chassis::wheelRotatePWM(int *speedPWM)
{
	int8_t w;
	
	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheel_move[w] = true;
		if (speedPWM[w] < 0) 
		{
			digitalWrite(_wheel_DIR_pins[w], HIGH);
			analogWrite(_wheel_PWM_pins[w], 255 + (speedPWM[w]%255));
		}
		else
		{
			digitalWrite(_wheel_DIR_pins[w], LOW);
			analogWrite(_wheel_PWM_pins[w], speedPWM[w]%255);
		}
	}
	return 0;
}

//==========================================
//==========================================
//==========================================

#if defined(ENCODERS_ON)
//========================================== wheelCalibrate

void Chassis::wheelCalibrate(int8_t wheel)
{
	unsigned long wheelTime = 0;
	this->_vbat = this->getVoltage();
	int _calib_wheel_table[WHEEL_COUNT][3][WHEEL_TABLE_SIZE];

	digitalWrite(_wheel_DIR_pins[wheel], LOW);
	digitalWrite(_wheel_PWM_pins[wheel], LOW);

	this->_wheelAngSpeed[wheel] = 0;

	unsigned int U = 0;
	
	while ((this->_wheelAngSpeed[wheel] == 0) && (U <= 255))
	{
		U++;
		wheelTime = 0;
		
		analogWrite(_wheel_PWM_pins[wheel], U);
		delay(WHEEL_TIME_MAX * 2 / 1000);
		
		for (int8_t i = MEAN_DEPTH-1; i >= 0; --i) wheelTime = wheelTime + _IRQ_wheelTimeArray[wheel][i];
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
	int8_t cstep = (255 - U) / (WHEEL_TABLE_SIZE - 1);
	for (int8_t i = 0; i < WHEEL_TABLE_SIZE; i++)
	{				
		if (U > 255) U = 255;
		wheelTime = 0;
		
		dist_wheelEncoderCount[wheel] = _IRQ_wheelEncoderCount[wheel] + 2 * WHEEL_SEGMENTS;
		analogWrite(_wheel_PWM_pins[wheel], U);
		while (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel]);
		if (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel])
		{
			i--;
			continue;
		}

		for (int8_t m = MEAN_DEPTH-1; m >= 0; --m) wheelTime = wheelTime + _IRQ_wheelTimeArray[wheel][m];
		analogWrite(_wheel_PWM_pins[wheel], 0);
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
		int8_t wheelBreakDelay = MAX_WHEEL_BREAK_DELAY;
		int8_t opt1_wheelBreakDelay = MAX_WHEEL_BREAK_DELAY;
		
		while (wheelBreakDelay > 0)
		{
			error_count = 0;
			dist_wheelEncoderCount[wheel] = _IRQ_wheelEncoderCount[wheel] + WHEEL_SEGMENTS;
			analogWrite(_wheel_PWM_pins[wheel], U);
			while (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel]);
			digitalWrite(_wheel_DIR_pins[wheel], !dir[wheel]);
			digitalWrite(_wheel_PWM_pins[wheel], dir[wheel]);
			delay(wheelBreakDelay);
			digitalWrite(_wheel_DIR_pins[wheel], dir[wheel]);
			delay(500);
			
			if (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel]) continue;
			
			error_count = _IRQ_wheelEncoderCount[wheel] - dist_wheelEncoderCount[wheel];
			if (min_error_count > error_count)
			{
				min_error_count = error_count;
				opt1_wheelBreakDelay = wheelBreakDelay;
			}
			wheelBreakDelay = wheelBreakDelay - MAX_WHEEL_BREAK_DELAY/5;
		}
		
		min_error_count = WHEEL_SEGMENTS;
		int8_t opt2_wheelBreakDelay = opt1_wheelBreakDelay + MAX_WHEEL_BREAK_DELAY/5;
		for (wheelBreakDelay = opt2_wheelBreakDelay; wheelBreakDelay > opt1_wheelBreakDelay - MAX_WHEEL_BREAK_DELAY/10; wheelBreakDelay = wheelBreakDelay - 2)
		{
			error_count = 0;                
			for (int8_t t = 5-1; t >= 0; --t)
			{
				dist_wheelEncoderCount[wheel] = _IRQ_wheelEncoderCount[wheel] + WHEEL_SEGMENTS;
				analogWrite(_wheel_PWM_pins[wheel], U);
				while (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel]);
				digitalWrite(_wheel_DIR_pins[wheel], !dir[wheel]);
				digitalWrite(_wheel_PWM_pins[wheel], dir[wheel]);
				delay(wheelBreakDelay);//Задержка для жесктого останова - именно на это время на двигатели подается обратное напряжение
				digitalWrite(_wheel_DIR_pins[wheel], dir[wheel]);
				delay(500);
				
				if (dist_wheelEncoderCount[wheel] > _IRQ_wheelEncoderCount[wheel])
				{
					t--;
					continue;
				}
				error_count = error_count + (_IRQ_wheelEncoderCount[wheel] - dist_wheelEncoderCount[wheel]);
			}
			if (min_error_count > error_count)
			{
				min_error_count = error_count;
				opt2_wheelBreakDelay = wheelBreakDelay;
			}	
		}
		
		_calib_wheel_table[wheel][VOLTS][i] = ((this->_vbat * U) / 255) * 100;
		_calib_wheel_table[wheel][SPEED][i] = this->_wheelAngSpeed[wheel];
		_calib_wheel_table[wheel][BREAKDELAY][i] = opt2_wheelBreakDelay;

#if defined (DEBUG_wheelCalibrate)
		Serial.print(F("DEBUG wheelCalibrate: "));
		Serial.print(F("_vbat("));
		Serial.print(((this->_vbat * U) / 255) * 100);
		Serial.print(F(") "));
		
		Serial.print(F("VOLTS("));
		Serial.print(_calib_wheel_table[wheel][VOLTS][i]);
		Serial.print(F(") "));
		
		Serial.print(F("SPEED("));
		Serial.print(_calib_wheel_table[wheel][SPEED][i]);
		Serial.print(F(") "));
		
		Serial.print(F("BDELAY("));
		Serial.print(_calib_wheel_table[wheel][BREAKDELAY][i]);
		Serial.print(F(") "));
		
		Serial.println();
#endif

		if (i < (WHEEL_TABLE_SIZE - 2)) U += cstep;
		else U = 255;
	}
	analogWrite(_wheel_PWM_pins[wheel], 0);
	
	this->_eepromWriteWheelTable(wheel, (int *)_calib_wheel_table[wheel]);
	
	return;
}

//========================================== getWheelTableValue

int Chassis::getWheelTableValue(int8_t wheel, int8_t parameter, int8_t record)
{
	unsigned int addr = EEPROM_WHEEL_TABLE_ADDR + (wheel*WHEEL_TABLE_SIZE*3 + record + parameter*WHEEL_TABLE_SIZE)*sizeof(int);

	int p;
	for (unsigned int i = 0; i < sizeof(int); i++) *((int8_t*)&p + i) = EEPROM.read(addr + i);

	return p;
}

//========================================== _eepromWriteWheelTable

int Chassis::_eepromWriteWheelTable(int8_t wheel, int *table)
{
	unsigned int N = WHEEL_TABLE_SIZE * 3 * sizeof(int);
	unsigned int addr = EEPROM_WHEEL_TABLE_ADDR + wheel*WHEEL_TABLE_SIZE*3*sizeof(int);
	
	for (unsigned int i = 0; i < N; i++) EEPROM.update(addr + i, *((int8_t*)table + i));

	return 0;
}

//========================================== _eepromReadWheelTable

int Chassis::_eepromReadWheelTable(int8_t wheel, int *table)
{
	unsigned int N = WHEEL_TABLE_SIZE * 3 * sizeof(int);
	unsigned int addr = EEPROM_WHEEL_TABLE_ADDR + wheel*WHEEL_TABLE_SIZE*3*sizeof(int);
	
	for (unsigned int i = 0; i < N; i++) *((int8_t*)table + i) = EEPROM.read(addr + i);

	return 0;
}

//========================================== _wheelGetU

int Chassis::_wheelGetU(float ang_speed, int wheel, float volts)
{
	unsigned int U = 0;
	float v = 0;
	int8_t i = 0;

	if (volts == 0) return -3;

	if ((ang_speed < (float)this->getWheelTableValue(wheel, SPEED, 0)) || (ang_speed > (float)getWheelTableValue(wheel, SPEED, WHEEL_TABLE_SIZE - 1))) return -1;

	for (i = WHEEL_TABLE_SIZE-1; i >= 0; --i)
	{
		if ((float)this->getWheelTableValue(wheel, SPEED, i) < ang_speed) break;
	}

	//(v-v1)/(v2-v1) = (s-s1)/(s2-s1) (s - SPEED (degrees/sec), v - VOLTS (V))
	//and -> v = ((s-s1)*(v2-v1)/(s2-s1)) + v1
	v = (float)this->getWheelTableValue(wheel, VOLTS, i + 1) + (ang_speed - (float)this->getWheelTableValue(wheel, SPEED, i + 1)) * ((float)this->getWheelTableValue(wheel, VOLTS, i) - (float)this->getWheelTableValue(wheel, VOLTS, i + 1)) / ((float)this->getWheelTableValue(wheel, SPEED, i) - (float)this->getWheelTableValue(wheel, SPEED, i + 1));
	U = (255 * v/100.0) / volts;

	if (U > 255) U = 255;
	
#if defined (DEBUG_wheelGetU)
	Serial.print(F("DEBUG _wheelGetU: "));
	Serial.print(F("wheel("));
	Serial.print(wheel);
	Serial.print(F(") "));
	Serial.print(F("volts("));
	Serial.print(volts);
	Serial.print(F(") "));
	// Serial.print(F("v("));
	// Serial.print(v);
	// Serial.print(F(") "));
	// Serial.print(F("i("));
	// Serial.print((int)i);
	// Serial.print(F(") "));
	Serial.print(F("ang_speed("));
	Serial.print(ang_speed);
	Serial.print(F(") | "));
	Serial.print(F("U("));
	Serial.print(U);
	Serial.println(F(")"));
#endif

	return U;
}

//========================================== _wheelGetBDelay

int Chassis::_wheelGetBDelay(float ang_speed, int wheel)
{
	unsigned int BDelay = 0;
	int8_t i = 0;

	if (fabs(ang_speed) < (float)this->getWheelTableValue(wheel, SPEED, 0)) return 0;
	if (fabs(ang_speed) > (float)this->getWheelTableValue(wheel, SPEED, WHEEL_TABLE_SIZE - 1)) return (float)this->getWheelTableValue(wheel, SPEED, WHEEL_TABLE_SIZE - 1);

	for (i = WHEEL_TABLE_SIZE-1; i >= 0; --i)
	{
		if ((float)this->getWheelTableValue(wheel, SPEED, i) < ang_speed) break;
	}
	i = i % (WHEEL_TABLE_SIZE-1);

	BDelay = this->getWheelTableValue(wheel, BREAKDELAY, i+1);

	return BDelay;
}

//========================================== wheelRotateAng

int Chassis::wheelRotateAng(float *speed, float *ang, bool en_break)
{
	int8_t w;
	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		if ((fabs(speed[w]) < (float)this->getWheelTableValue(w, SPEED, 0)) && (fabs(speed[w]) > 0)) return -1; //ошибка - невозможно совершить поворот колеса с такой маленькой скоростью
		if ((fabs(ang[w]) < float(SEGMENT_ANGLE)) && (fabs(speed[w]) > 0)) return -2; //ошибка - невозможно совершить поворот колеса на такой маленький угол
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		if (speed[w] != 0) break;
	}
	
	if (w == WHEEL_COUNT) //Stop motors
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			analogWrite(_wheel_PWM_pins[w], 0);
			digitalWrite(_wheel_DIR_pins[w], LOW);
			this->_wheelSetAng[w] = 0;
			this->_wheelSetAngSpeed[w] = 0;
			this->_wheel_move[w] = false;
		}
		this->_wheel_sync_move = false;
		return 0;
	}
	else
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			this->_wheelSetAng[w] = ang[w];
			this->_wheelSetAngSpeed[w] = speed[w];
		}
		this->_wheel_sync_move = true;
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheelDir[w] = 1;
		if ((this->_wheelSetAng[w] * this->_wheelSetAngSpeed[w]) < 0) this->_wheelDir[w] = -1;

		dist_wheelEncoderCount[w] = _IRQ_wheelEncoderCount[w] + int(0.0 + (fabs(this->_wheelSetAng[w]) / float(SEGMENT_ANGLE))); //число сегментов маркерной линейки колеса, которое нужно отсчитать, чтобы повернуть на требуемый угол;
		this->_vbat = this->getVoltage();
		U[w] = this->_wheelGetU(fabs(this->_wheelSetAngSpeed[w]), w, this->_vbat);
		if (U[w] < 0) return -1;
		U[w] = (float)this->_wheelDir[w] * U[w];
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		memset((void*)_IRQ_wheelTimeArray[w], 0, sizeof(long) * MEAN_DEPTH);

		dir[w] = false;
		if (this->_wheelDir[w] < 0) dir[w] = true;

		this->_wheelSetAngSpeed[w] = (float)this->_wheelDir[w] * fabs(this->_wheelSetAngSpeed[w]);

		digitalWrite(_wheel_DIR_pins[w], dir[w]);
		digitalWrite(_wheel_PWM_pins[w], dir[w]);
		this->_wheel_move[w] = true;
	}

	this->_wheel_rotate_sync(en_break);

	for (w = WHEEL_COUNT-1; w >= 0; --w) this->_wheel_move[w] = false;

	return 0;
}

//========================================== wheelRotate

int Chassis::wheelRotate(float *speed)
{
	int8_t w;
	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		if ((fabs(speed[w]) < (float)this->getWheelTableValue(w, SPEED, 0)) && (fabs(speed[w]) != 0)) return -1; //ошибка - невозможно совершить поворот колеса с такой маленькой скоростью
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		if (speed[w] != 0) break;
	}
	
	if (w == WHEEL_COUNT) //Stop motors
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			analogWrite(_wheel_PWM_pins[w], 0);
			digitalWrite(_wheel_DIR_pins[w], LOW);
			this->_wheelSetAngSpeed[w] = 0;
			this->_wheel_move[w] = false;
		}
		this->_wheel_sync_move = false;
		return 0;
	}
	else
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w) this->_wheelSetAngSpeed[w] = speed[w];
		this->_wheel_sync_move = true;
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		this->_wheelDir[w] = 1;
		if (this->_wheelSetAngSpeed[w] < 0) this->_wheelDir[w] = -1;

		this->_vbat = this->getVoltage();
		U[w] = this->_wheelGetU(fabs(this->_wheelSetAngSpeed[w]), w, this->_vbat);
		if (U[w] < 0) return -1;
		U[w] = (float)this->_wheelDir[w] * U[w];
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		dir[w] = false;
		if (this->_wheelDir[w] < 0) dir[w] = true;

		this->_wheelSetAngSpeed[w] = (float)this->_wheelDir[w] * fabs(this->_wheelSetAngSpeed[w]);

		digitalWrite(_wheel_DIR_pins[w], dir[w]);
		digitalWrite(_wheel_PWM_pins[w], dir[w]);
		this->_wheel_move[w] = true;
	}

	return 0;
}

//========================================== wheelGetPath
float Chassis::wheelGetPath(int8_t wheel) { return 2 * MIRO_PI * WHEEL_RADIUS * ((float)_IRQ_wheelEncoderCount[wheel] / WHEEL_SEGMENTS); }

//========================================== wheelGetEncoder
unsigned long Chassis::wheelGetEncoder(int8_t wheel) { return _IRQ_wheelEncoderCount[wheel]; }

//========================================== wheelResetEncoder
void Chassis::wheelResetEncoder(int8_t wheel) { _IRQ_wheelEncoderCount[wheel] = 0; }

//++++++++++++++++++++++++++++++++++++++++++
//PRIVATE
//++++++++++++++++++++++++++++++++++++++++++

void Chassis::_wheel_rotate_sync(bool en_break)
{
	unsigned long wheelTime = 0;
	int8_t break_wheel = 0;
	bool complete = false;
	int8_t w;
	
#if defined(DEBUG_WHEEL_PID)
	debug_wheel_pid_max_error = 0;
	pid_i = 0;
#endif
	
	while (1)
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			if (dist_wheelEncoderCount[w] <= _IRQ_wheelEncoderCount[w]) 
			{
				complete = true;
				break_wheel = w;
				break; //Exit from "for". Rotate finished.
			}
		}
		if (complete) break; //Exit from "while" if rotate finished

		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			if (_IRQ_syncloop[w])
			{
				_IRQ_syncloop[w] = false;
				wheelTime = 0;
				int8_t k = 0;
				
				for (int8_t i = MEAN_DEPTH-1; i >= 0; --i)
				{
					if ((_IRQ_wheelTimeArray[w][i] <= wheel_time_max[w]) && (_IRQ_wheelTimeArray[w][i] >= wheel_time_min[w]))
					{
						wheelTime = wheelTime + _IRQ_wheelTimeArray[w][i];
						k++;
					}
				}
				this->_wheelAngSpeed[w] = 0;
#if defined (DEBUG_wheel_rotate_sync)
				Serial.print(F("DEBUG _wheel_rotate_sync: "));
				Serial.print(F("k("));
				Serial.print((int)k);
				Serial.print(F(") "));
				Serial.print(F("wheelTime("));
				Serial.print(wheelTime);
				Serial.print(F(") "));
#endif
				if (k > 0)
				{
					wheelTime = wheelTime / k;
					this->_wheelAngSpeed[w] = (360000000.0/WHEEL_SEGMENTS) / wheelTime;
					this->_wheelAngSpeed[w] = this->_wheelAngSpeed[w] * (float)this->_wheelDir[w];
				}

				Error[w] = (this->_wheelSetAngSpeed[w] - this->_wheelAngSpeed[w]);
				U[w] = U[w] + DEFAULT_PID_Kp * Error[w];
				U[w] = U[w] + DEFAULT_PID_Kd * (Error[w] - Error1[w]);
				Error1[w] = Error[w];
				if (U[w] > 255) U[w] = 255;
				if (U[w] < -255) U[w] = -255;
				U0[w] = abs((int)U[w]);
				if (dir[w]) U0[w] = 255 - U0[w];
				
#if defined(DEBUG_wheel_rotate_sync)
				Serial.print(F("break_wheel("));
				Serial.print((int)break_wheel);
				Serial.print(F(") "));
				Serial.print(F("Error["));
				Serial.print((int)w);
				Serial.print(F("]("));
				Serial.print(Error[w]/this->_wheelSetAngSpeed[w]);
				Serial.print(F(") "));
				Serial.print(F("U["));
				Serial.print((int)w);
				Serial.print(F("]("));
				Serial.print(U[w]);
				Serial.println(F(") "));
#endif
				
#if defined(DEBUG_WHEEL_PID)
				if ((w == 0) && (pid_i < DEBUG_WHEEL_PID_I_DEPTH) && (k > 0))
				{
					debug_wheel_pid[pid_i] = Error[0]/this->_wheelSetAngSpeed[0];
					if (fabs(Error[0]/this->_wheelSetAngSpeed[0]) > debug_wheel_pid_max_error) debug_wheel_pid_max_error = fabs(Error[0]/this->_wheelSetAngSpeed[0]);
					pid_i++;
					
				}
#endif
			}
		}	
		for (w = WHEEL_COUNT-1; w >= 0; --w) analogWrite(_wheel_PWM_pins[w], U0[w]);
	}
	
	if (en_break)
	{
		for (w = WHEEL_COUNT-1; w >= 0; --w)
		{
			digitalWrite(_wheel_DIR_pins[w], !dir[w]);
			digitalWrite(_wheel_PWM_pins[w], dir[w]);
		}
		delay(WHEEL_BREAK_DELAY_MULT*this->_wheelGetBDelay(this->_wheelSetAngSpeed[break_wheel], 0));
	}
	
	for (w = WHEEL_COUNT-1; w >= 0; --w) 
	{
		digitalWrite(_wheel_DIR_pins[w], dir[w]);
		digitalWrite(_wheel_PWM_pins[w], dir[w]);
	}

	for (w = WHEEL_COUNT-1; w >= 0; --w)
	{
		dist_wheelEncoderCount[w] = _IRQ_wheelEncoderCount[w];
		this->_wheelAngSpeed[w] = 0;
		this->_wheelSetAngSpeed[w] = 0;
	}
#if defined(DEBUG_WHEEL_PID)
	for (int i = 0; i < pid_i; i++)
	{
		//int m = fabs(70 * debug_wheel_pid[i] / debug_wheel_pid_max_error); //unnormalized bars
		int m = fabs(70 * debug_wheel_pid[i] / 1); // normalized bars
		for (int k = 0; k < m; k++) Serial.print(F("\xDB"));
		Serial.print(F("|"));
		Serial.println(debug_wheel_pid[i]);
	}
#endif

}

#endif // ENCODERS_ON