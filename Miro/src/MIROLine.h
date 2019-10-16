#include "Device.h"

namespace miro {

class MIROLine : virtual public Device {
public:
    void GetParam(byte bnum, byte *pvalue);	
	void Init(byte pin);
	
	byte GetPinsCount();
	char* GetName();
	byte GetParamCount();

	float getVoltage(float vref);
	int getValue();
};

} //end namespace