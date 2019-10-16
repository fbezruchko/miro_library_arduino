#include "Device.h"

namespace miro {

class MIROLed : virtual public Device {
public:
	void SetParam(byte bnum, byte *pvalue);
    void GetParam(byte bnum, byte *pvalue);
	
	byte GetPinsCount();
	char* GetName();
	byte GetParamCount();
	
	void Init(byte pin);
	void On(byte value);
	void On();
	void Off();
private:
    byte value;
};

}