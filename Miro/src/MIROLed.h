#include "Device.h"

//namespace miro {

class MIROLed : virtual public Device {
public:
	void setParam(byte bnum, byte *pvalue);
    void getParam(byte bnum, byte *pvalue);
	
	byte getPinsCount();
	char* getName();
	byte getParamCount();
	
	void Init(byte pin);
	void On(byte value);
	void On();
	void Off();
private:
    byte value;
};

//}