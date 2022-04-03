#ifndef chassis_irq_h
#define chassis_irq_h

#include "Arduino.h"

//namespace miro {

static volatile unsigned long _IRQ_wheelTimeArray[WHEEL_COUNT][MEAN_DEPTH];
static volatile long _IRQ_wheelTime[WHEEL_COUNT];
static volatile long _IRQ_wheelTime_last[WHEEL_COUNT];
static volatile unsigned int _IRQ_wheelEncoderCount[WHEEL_COUNT];
static volatile bool _IRQ_syncloop[WHEEL_COUNT] = {true, true};

static void wheelISR0()
{
	cli();
	_IRQ_wheelTime[0] = micros();
	//_IRQ_wheelTimeArray[0][_IRQ_wheelEncoderCount[0] % MEAN_DEPTH] = _IRQ_wheelTime[0] - _IRQ_wheelTime_last[0];
	_IRQ_wheelTimeArray[0][*((uint8_t *)(&_IRQ_wheelEncoderCount[0])) % MEAN_DEPTH] = _IRQ_wheelTime[0] - _IRQ_wheelTime_last[0];
	_IRQ_wheelTime_last[0] = _IRQ_wheelTime[0];
	_IRQ_wheelEncoderCount[0]++;
	_IRQ_syncloop[0] = true;
	sei();
}

static void wheelISR1()
{
	cli();
	_IRQ_wheelTime[1] = micros();
	//_IRQ_wheelTimeArray[1][_IRQ_wheelEncoderCount[1] % MEAN_DEPTH] = _IRQ_wheelTime[1] - _IRQ_wheelTime_last[1];
	_IRQ_wheelTimeArray[1][*((uint8_t *)(&_IRQ_wheelEncoderCount[1])) % MEAN_DEPTH] = _IRQ_wheelTime[1] - _IRQ_wheelTime_last[1];
	_IRQ_wheelTime_last[1] = _IRQ_wheelTime[1];
	_IRQ_wheelEncoderCount[1]++;
	_IRQ_syncloop[1] = true;
	sei();
}

static void (*wheelISR[WHEEL_COUNT])() = {wheelISR0, wheelISR1};

//} // end namespace

#endif