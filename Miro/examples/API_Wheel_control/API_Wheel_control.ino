#include <Led.h>
#include <Miro.h>
#include <Chassis.h>

Miro robot;

void setup() {
  // put your setup code here, to run once:
  robot.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  //robot.chassis.wheelCalibrate();;
  robot.Sync();
}
