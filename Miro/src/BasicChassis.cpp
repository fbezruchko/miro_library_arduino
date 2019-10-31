#include "BasicChassis.h"

#include "math.h"
#include "EEPROM.h"

using namespace miro;

//++++++++++++++++++++++++++++++++++++++++++
//PUBLIC
//++++++++++++++++++++++++++++++++++++++++++

//========================================== Init

void BasicChassis::Init(byte *PWM_pins, byte *DIR_pins)
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
}

//========================================== GetVoltage

float BasicChassis::getVoltage()
{
	byte n = 10;
	float v = 0;
	for (byte i = 0; i < n; i++)
	{
		v = v + analogRead(PIN_VBAT);
	}
	v = VOLTAGE_DIVIDER * 5.0 * ((v / n) / 1023.0);

	return v;
}

//========================================== wheelRotatePWMTime

int BasicChassis::wheelRotatePWMTime(int *speedPWM, unsigned long time)
{
	byte w;
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if (abs(speedPWM[w]) > 255) return -1;
	}
	
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		this->_wheel_move[w] = true;
		if (speedPWM[w] < 0) 
		{
			digitalWrite(wheel_DIR_pins[w], HIGH);
			analogWrite(wheel_PWM_pins[w], 255 - speedPWM[w]);
		}
		else
		{
			digitalWrite(wheel_DIR_pins[w], LOW);
			analogWrite(wheel_PWM_pins[w], speedPWM[w]);
		}
	}
	delay(time);
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		digitalWrite(wheel_DIR_pins[w], LOW);
		analogWrite(wheel_PWM_pins[w], 0);
		this->_wheel_move[w] = false;
	}
	
	return 0;
}

//========================================== wheelRotatePWM

int BasicChassis::wheelRotatePWM(int *speedPWM)
{
	byte w;
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		if (abs(speedPWM[w]) > 255) return -1;
	}
	
	for (w = 0; w < WHEEL_COUNT; w++)
	{
		this->_wheel_move[w] = true;
		if (speedPWM[w] < 0) 
		{
			digitalWrite(wheel_DIR_pins[w], HIGH);
			analogWrite(wheel_PWM_pins[w], 255 - speedPWM[w]);
		}
		else
		{
			digitalWrite(wheel_DIR_pins[w], LOW);
			analogWrite(wheel_PWM_pins[w], speedPWM[w]);
		}
	}
	return 0;
}
