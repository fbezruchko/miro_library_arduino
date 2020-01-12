#include <MIROLed.h>

MIROLed robot_Led_L;
MIROLed robot_Led_R;

void setup() {
  // put your setup code here, to run once:  
  robot_LedL.Init(9);
  robot_LedR.Init(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  robot_LedL.On();
  robot_LedR.Off();
  delay(500);
  robot_LedL.Off();
  robot_LedR.On();
  delay(500);
}
