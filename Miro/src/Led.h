#include "Device.h"

namespace miro {

class Led : virtual public Device {
public:
	void SetParam(byte bnum, float pvalue);
    void GetParam(byte bnum, byte *pvalue);
	
	void Init(byte pin);
	void On(byte value);
	void On();
	void Off();
private:
    byte led_value;
};

}