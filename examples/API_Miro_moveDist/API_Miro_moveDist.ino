#include <Miro.h>

using namespace miro;

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
byte ENCODER_pins[2] = { 2, 3 };
Miro robot(PWM_pins, DIR_pins, ENCODER_pins);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(115200);
}

int laps = 0;

void loop() {
// put your main code here, to run repeatedly:
	for (unsigned char i = 0; i < 4; i++)
	{
		robot.moveDist(robot.getOptLinSpeed(), 0, 1, true);
		delay(500);
		robot.rotateAng(0.5*robot.getOptAngSpeed(), -90, true);
		delay(500);
	}
	Serial.print("Laps: ");
	Serial.println(laps);
	laps++;
}
