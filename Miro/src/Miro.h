#ifndef miro_h
#define miro_h

#include "Chassis.h"
#include "Device.h"

const char MIRO_VERSION[] = "v1.0.0";
#define MIRO_MAX_DEVICES 16	//Максимальное число устройств, которые можно подключить к роботу.

namespace miro {

class Miro {
public:

	Chassis chassis;
	
	void Init();
	
	void Sync();

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

	/*подключение устройства к роботу*/
	int attachDevice(Device *dev);

	int dettachDevice(Device *dev);
	int dettachDevice(byte DeviceIndex);

    Device* GetDeviceByIndex(byte DeviceIndex);
    byte GetDeviceCount();
	
private:

	Device* devices[MIRO_MAX_DEVICES];
	byte _device_count;
};

} // end namespace

#endif
