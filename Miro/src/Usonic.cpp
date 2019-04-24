#include "Usonic.h"

#define TRIG_PIN 0
#define ECHO_PIN 1

#define NUM 0
#define TYPE 1

void Usonic::On(unsigned int max_dist)
{
	this->_isOn = true;
	this->_max_dist = max_dist;
}

void Usonic::On()
{
	this->_isOn = true;
	this->_max_dist = MAX_DIST;
}

void Usonic::Off()
{
	this->_isOn = false;
}

void Usonic::Sync()
{
	if (this->_isOn)
	{
		digitalWrite(this->pins[TRIG_PIN][NUM], LOW);
		delayMicroseconds(2);
		digitalWrite(this->pins[TRIG_PIN][NUM], HIGH);
		delayMicroseconds(10);
		digitalWrite(this->pins[TRIG_PIN][NUM], LOW);
		this->_dist = pulseIn(this->pins[ECHO_PIN][NUM], HIGH, this->_max_dist * 5.8);
		this->_dist = this->_dist / 5.8;
		if (this->_dist > this->_max_dist) this->_dist = this->_max_dist+1;
		this->_mesCount++;
	}
}

void Usonic::Init(byte trig_pin, byte echo_pin)
{
	this->pins_count = 2;
	this->pins[TRIG_PIN][NUM] = trig_pin;
	this->pins[TRIG_PIN][TYPE] = OUTPUT;
	this->pins[ECHO_PIN][NUM] = echo_pin;
	this->pins[ECHO_PIN][TYPE] = INPUT;
	pinMode(trig_pin, OUTPUT);
	pinMode(echo_pin, INPUT);

	this->_dist = 0;
	this->_mesCount = 0;
	this->_max_dist = MAX_DIST;
	this->_isOn = false;
}

int Usonic::getDist(unsigned int max_dist)
{
	if (!this->_isOn)
	{
		digitalWrite(this->pins[TRIG_PIN][NUM], LOW);
		delayMicroseconds(2);
		digitalWrite(this->pins[TRIG_PIN][NUM], HIGH);
		delayMicroseconds(10);
		digitalWrite(this->pins[TRIG_PIN][NUM], LOW);
		this->_dist = pulseIn(this->pins[ECHO_PIN][NUM], HIGH, max_dist * 5.8);
		this->_dist = this->_dist / 5.8;
		if ((this->_dist > max_dist) || (this->_dist == 0)) return -1;
		this->_mesCount++;
	}

	return this->_dist;
}
