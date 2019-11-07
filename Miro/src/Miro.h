#pragma once

#include "Chassis.h"
#include "Device.h"
#include "Robot.h"

#include "defs.h"
#include "config.h"
#include "default_config.h"

//namespace miro {

const char MIRO_VERSION[] = "v1.0.0";

class Miro : public Robot {
public:

	Miro(byte *PWM_pins, byte *DIR_pins);
#if defined(ENCODERS_ON)
	Miro(byte *PWM_pins, byte *DIR_pins, byte *ENCODER_pins);
#endif
	~Miro();
	
	/*движение робота с прямым управлением PWM.
	PWM_lin_speed - значение PWM, задающего линейную скорость робота [-255..255],
	PWM_ang_speed - разница в значениях PWM, определяющих угловую скорость робота [-255..255],
	time - время воздействия сигнала PWM (millis),
	*/
	int movePWMTime(int PWM_lin_speed, int PWM_angle_speed, unsigned long time);
	
	/*движение робота с прямым управлением PWM.
	PWM_lin_speed - значение PWM, задающего линейную скорость робота [-255..255],
	PWM_ang_speed - разница в значениях PWM, определяющих угловую скорость робота [-255..255],
	*/
	int movePWM(int PWM_lin_speed, int PWM_angle_speed);
	
	int rotatePWMTime(int PWM_speed, unsigned long time);
	int rotatePWM(int PWM_speed);

#if defined(ENCODERS_ON)
	/*движение робота.
	lin_speed - линейная скорость робота (м/с),
	ang_speed - угловая скорость робота (град/сек),
	dist - длина пути, которую нужно преодолеть роботу (м),
	*/
	int moveDist(float lin_speed, float ang_speed, float dist, bool en_break); //Движение робота с заданными линейной и угловыми скоростями
	
	/*движение робота.
	lin_speed - линейная скорость робота (м/с),
	ang_speed - угловая скорость робота (град/сек),
	*/
	int move(float lin_speed, float ang_speed); //Движение робота с заданными линейной и угловыми скоростями

	/*поворот робота на месте.
	ang - угол поворота,
	ang_speed - угловая скорость поворота (град/сек),
	*/
	int rotateAng(float ang_speed, float ang, bool en_break);
	
	/*поворот робота на месте.
	ang_speed - угловая скорость поворота (град/сек),
	*/
	int rotate(float ang_speed);

	/*возвращает угловую скороть шасси (рад/сек).*/
	float getAngSpeedRad();

	/*возвращает угловую скороть шасси (град/сек).*/
	float getAngSpeed();

	/*возвращает линейную скороть шасси (м/с).*/
	float getLinSpeed();

	/*возвращает длину пройденного шасси пути.*/
	float getPath(); //Не реализовано нормально - при расчете учитываются повороты на месте, когда центр масс робота фактически никуда не движется
	
	float getMaxLinSpeed();
	float getOptLinSpeed();
	float getMinLinSpeed();
	
	float getMaxAngSpeed();
	float getOptAngSpeed();
	float getMinAngSpeed();
#endif
};

//} // end namespace
