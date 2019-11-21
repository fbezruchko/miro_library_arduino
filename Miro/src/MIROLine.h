#include "Device.h"

//namespace miro {

class MIROLine : virtual public Device {
public:
    void getParam(byte bnum, byte *pvalue);	
	void Init(byte pin);
	
	byte getPinsCount();
	char* getName();
	byte getParamCount();

	float getVoltage(float vref);
	int getValue();
};

//} //end namespace