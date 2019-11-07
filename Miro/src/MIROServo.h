#include "Device.h"
#include <Servo.h>

//namespace miro {

class MIROServo : virtual public Device {
public:
	void SetParam(byte bnum, byte *pvalue);
    void GetParam(byte bnum, byte *pvalue);
	
	byte GetPinsCount();
	char* GetName();
	byte GetParamCount();
	
	void Init(byte pin, byte min_value, byte max_value);
	int Pos(byte value);
	byte GetMin() {return _min_value;}
	byte GetMax() {return _max_value;}
private:
	Servo servo0;
	byte _min_value;
	byte _max_value;
};

//}