#include "Device.h"

#define MAX_DIST 4000 //4 метра

class Usonic : virtual public Device {
public:
	void Sync();
	void Init(byte trig_pin, byte echo_pin);
	unsigned int getMesCount();
	int getDist(unsigned int max_dist);
	void On();
	void On(unsigned int max_dist);
	void Off();

private:
	bool _isOn;
	unsigned int _mesCount;
	unsigned int _dist;
	unsigned int _max_dist;
};
