#include <Led.h>
#include <Ldr.h>
#include <Miro.h>

#define LDR_TH 300

Miro robot;
Led robot_LedL;
Led robot_LedR;

Ldr robot_LdrL;
Ldr robot_LdrR;

void setup() {
  // put your setup code here, to run once:
  robot.Init();
  
  robot_LedL.Init(9);
  robot_LedR.Init(10);

  robot_LdrL.Init(A2);
  robot_LdrR.Init(A3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (robot_LdrL.getValue() < LDR_TH) robot_LedL.Off(); else robot_LedL.On();
  if (robot_LdrR.getValue() < LDR_TH) robot_LedR.Off(); else robot_LedR.On();
  
  delay(50);
}
