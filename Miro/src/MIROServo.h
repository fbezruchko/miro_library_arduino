#include "Device.h"
#include <Servo.h>

//namespace miro {

class MIROServo : virtual public Device {
public:
	void setParam(byte bnum, byte *pvalue);
    void getParam(byte bnum, byte *pvalue);
	
	byte getPinsCount();
	char* getName();
	byte getParamCount();
	
	void Init(byte pin, byte min_value, byte max_value);
	int Pos(byte value);
	byte getMin() {return _min_value;}
	byte getMax() {return _max_value;}
private:
	Servo servo0;
	byte _min_value;
	byte _max_value;
};

//}