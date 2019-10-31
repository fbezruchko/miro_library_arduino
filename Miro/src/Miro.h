#ifndef miro_h
#define miro_h

#include "chassis_config.h"

#if defined (CHASSIS_W_ENCODERS)
	#include "SyncChassis.h"
#else
	#include "BasicChassis.h"
#endif

#include "Device.h"

namespace miro {

const char MIRO_VERSION[] = "v1.0.0";
#define MIRO_MAX_DEVICES 16	//Максимальное число устройств, которые можно подключить к роботу.

class Miro {
public:

	Miro(byte *PWM_pins, byte *DIR_pins);

	#if defined (CHASSIS_W_ENCODERS)
		SyncChassis chassis;
	#else
		BasicChassis chassis;
	#endif
	
	void Sync();
	
	/*движение робота с прямым управлением PWM.
	PWM_lin_speed - значение PWM, задающего линейную скорость робота [-255..255],
	PWM_ang_speed - разница в значениях PWM, определяющих угловую скорость робота [-255..255],
	time - время воздействия сигнала PWM (millis),
	*/
	int MovePWMTime(int PWM_lin_speed, int PWM_angle_speed, unsigned long time);
	
	/*движение робота с прямым управлением PWM.
	PWM_lin_speed - значение PWM, задающего линейную скорость робота [-255..255],
	PWM_ang_speed - разница в значениях PWM, определяющих угловую скорость робота [-255..255],
	*/
	int MovePWM(int PWM_lin_speed, int PWM_angle_speed);
	
	int RotatePWMTime(int PWM_speed, unsigned long time);
	int RotatePWM(int PWM_speed);

	/*подключение устройства к роботу*/
	int attachDevice(Device *dev);

	int dettachDevice(Device *dev);
	int dettachDevice(byte DeviceIndex);

    Device* GetDeviceByIndex(byte DeviceIndex);
    byte GetDeviceCount();

#if defined (CHASSIS_W_ENCODERS)
	/*движение робота.
	lin_speed - линейная скорость робота (м/с),
	ang_speed - угловая скорость робота (град/сек),
	dist - длина пути, которую нужно преодолеть роботу (м),
	*/
	int MoveDist(float lin_speed, float ang_speed, float dist, bool en_break); //Движение робота с заданными линейной и угловыми скоростями
	
	/*движение робота.
	lin_speed - линейная скорость робота (м/с),
	ang_speed - угловая скорость робота (град/сек),
	*/
	int Move(float lin_speed, float ang_speed); //Движение робота с заданными линейной и угловыми скоростями

	/*поворот робота на месте.
	ang - угол поворота,
	ang_speed - угловая скорость поворота (град/сек),
	*/
	int RotateAng(float ang_speed, float ang, bool en_break);
	
	/*поворот робота на месте.
	ang_speed - угловая скорость поворота (град/сек),
	*/
	int Rotate(float ang_speed);

	/*возвращает угловую скороть шасси (рад/сек).*/
	float getAngSpeedRad();

	/*возвращает угловую скороть шасси (град/сек).*/
	float getAngSpeed();

	/*возвращает линейную скороть шасси (м/с).*/
	float getLinSpeed();

	/*возвращает длину пройденного шасси пути.*/
	float getPath(); //Не реализовано нормально - при расчете учитываются повороты на месте, когда центр масс робота фактически никуда не движется
#endif
	
private:

	Device* devices[MIRO_MAX_DEVICES];
	byte _device_count;
};

} // end namespace

#endif
