#include "Device.h"

namespace miro {

class MIROUsonic : virtual public Device {
public:
	void Sync();
	void SetParam(byte bnum, byte *pvalue);
    void GetParam(byte bnum, byte *pvalue);
	byte GetPinsCount();
	char* GetName();
	byte GetParamCount();
	
	void Init(byte trig_pin, byte echo_pin);
	void On(unsigned int max_dist);
	void On();
	void Off();

	int getDist(unsigned int max_dist);
	unsigned int getMesCount();
private:
	bool _isOn;
	unsigned int _mesCount;
	unsigned int _dist;
	unsigned int _max_dist;
};

} //end namespace