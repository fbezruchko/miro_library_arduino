#include <Miro.h>

Miro robot;

void setup() {
  // put your setup code here, to run once:
  robot.Init();
  robot.chassis.wheelCalibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
}
