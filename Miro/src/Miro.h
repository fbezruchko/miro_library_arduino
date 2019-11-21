#pragma once

#include "Chassis.h"
#include "Device.h"
#include "Robot.h"

#include "defs.h"
#include "config.h"
#include "default_config.h"

#include <avr/pgmspace.h>

namespace miro {

const char MIRO_VERSION[] = "v1.0.0";

class Miro : public Robot {
public:

	Miro(byte *PWM_pins, byte *DIR_pins);
#if defined(ENCODERS_ON)
	Miro(byte *PWM_pins, byte *DIR_pins, byte *ENCODER_pins);
#endif  // ENCODERS_ON
	~Miro();
	
	/*robot movement with simple PWM control and time contraint.
	PWM_lin_speed - PWM value [-255..255],
	PWM_ang_speed - PWM values different [-255..255] (_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2; and _wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;),
	time - time of movement (millis).
	*/
	int movePWMTime(int PWM_lin_speed, int PWM_angle_speed, unsigned long time);
	
	/*robot movement with simple PWM control
	PWM_lin_speed - PWM value [-255..255],
	PWM_ang_speed - PWM values different [-255..255] (_wheelSetPWM[LEFT] = PWM_lin_speed - PWM_angle_speed / 2; and _wheelSetPWM[RIGHT] = PWM_lin_speed + PWM_angle_speed / 2;),
	*/
	int movePWM(int PWM_lin_speed, int PWM_angle_speed);
	
	int rotatePWMTime(int PWM_speed, unsigned long time);
	int rotatePWM(int PWM_speed);

#if defined(ENCODERS_ON)
	/*robot movement with linear, angular speed and distance control
	lin_speed - robot linear speed (m/s),
	ang_speed - robot angular speed (degrees/s),
	dist - distance (m),
	en_break - TRUE/FALSE break in the and of distance
	*/
	int moveDist(float lin_speed, float ang_speed, float dist, bool en_break);
	
	/*robot movement with linear, angular speed control
	lin_speed - robot linear speed (m/s),
	ang_speed - robot angular speed (degrees/s),
	*/
	int move(float lin_speed, float ang_speed);

	/*robot rotation with angular speed and angle control
	ang - rotation angle,
	ang_speed - robot angular speed (degrees/s),
	en_break - TRUE/FALSE break in the and of rotation
	*/
	int rotateAng(float ang_speed, float ang, bool en_break);
	
	/*robot rotation with angular speed control
	ang_speed - robot angular speed (degrees/s)
	*/
	int rotate(float ang_speed);

	/*returns robot angular speed (radians)*/
	float getAngSpeedRad();

	/*returns robot angular speed (degrees/s)*/
	float getAngSpeed();

	/*returns robot linear speed (m/s).*/
	float getLinSpeed();

	/*returns full path or robot from power up (m)*/
	float getPath(); //need good implementation - current implementation consider path of robot rotation
	
	float getMaxLinSpeed();
	float getOptLinSpeed();
	float getMinLinSpeed();
	
	float getMaxAngSpeed();
	float getOptAngSpeed();
	float getMinAngSpeed();
#endif // ENCODERS_ON
};

} // end namespace
