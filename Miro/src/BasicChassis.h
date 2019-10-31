#ifndef basic_chassis_h
#define basic_chassis_h

#include "chassis_config.h"

namespace miro {

class BasicChassis {
public:
	/*Init pins, modes, irq's - must call in setup()*/
	void Init(byte *PWM_pins, byte *DIR_pins);
	
	void Sync() { return; }

	/*Battery voltage measurement*/
	float getVoltage();
	
	/*
	Wheel rotation at a given PWM [-255..255] for a time in millis.
	*/
	int wheelRotatePWMTime(int *speedPWM, unsigned long time);
	
	/*
	Wheel rotation at a given PWM [-255..255].
	The return from the function will happen immediately.
	*/
	int wheelRotatePWM(int *speedPWM);
	
	/*Returns True if the wheel is spinning and False if it is not spinning*/
	bool wheelIsMoving(byte wheel) {return this->_wheel_move[wheel];}
	
	byte getWheelCount() {return WHEEL_COUNT;}

protected:

	float _vbat; //Battery volgage
	bool _wheel_move[WHEEL_COUNT];
	char _wheelDir[WHEEL_COUNT];

	byte wheel_PWM_pins[WHEEL_COUNT];
	byte wheel_DIR_pins[WHEEL_COUNT];
	bool _wheel_sync_move;

};

} // end namespace

#endif
