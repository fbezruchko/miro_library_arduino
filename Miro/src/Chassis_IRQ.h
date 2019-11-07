#pragma once

//namespace miro {

static volatile unsigned long _wheelTimeArray[WHEEL_COUNT][MEAN_DEPTH];
static volatile long _wheelTime[WHEEL_COUNT];
static volatile long _wheelTime_last[WHEEL_COUNT];
static volatile unsigned int _wheelEncoderCount[WHEEL_COUNT];
static volatile bool _syncloop[WHEEL_COUNT] = { true, true };

static void wheelISR0()
{
	cli();
	_wheelTime[0] = micros();
	_wheelTimeArray[0][_wheelEncoderCount[0] % MEAN_DEPTH] = _wheelTime[0] - _wheelTime_last[0];
	_wheelTime_last[0] = _wheelTime[0];
	_wheelEncoderCount[0]++;
	_syncloop[0] = true;
	sei();
}

static void wheelISR1()
{
	cli();
	_wheelTime[1] = micros();
	_wheelTimeArray[1][_wheelEncoderCount[1] % MEAN_DEPTH] = _wheelTime[1] - _wheelTime_last[1];
	_wheelTime_last[1] = _wheelTime[1];
	_wheelEncoderCount[1]++;
	_syncloop[1] = true;
	sei();
}

static void(*wheel_ISR[WHEEL_COUNT])() = { wheelISR0, wheelISR1 };

//} // end namespace