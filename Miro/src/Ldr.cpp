#include "Ldr.h"

#define NUM 0
#define TYPE 1

using namespace miro;

void Ldr::Init(byte pin)
{
	this->pins_count = 1;
	this->pins[0][NUM] = pin;
	this->pins[0][TYPE] = INPUT;
	pinMode(pin, INPUT);
}

int Ldr::getValue()
{
	return analogRead(this->pins[0][NUM]);
}

float Ldr::getVoltage(float vref)
{
	return vref*((analogRead(this->pins[0][NUM]))/1023);
}
