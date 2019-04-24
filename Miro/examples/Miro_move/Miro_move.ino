#include <Miro.h>

Miro robot;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  robot.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long lcount = robot.chassis.wheelGetTachom(LEFT);
  unsigned long rcount = robot.chassis.wheelGetTachom(RIGHT);

  Serial.print("Left odometry sensor: ");
  Serial.println(lcount);
  
  Serial.print("Rigth odometry sensor: ");
  Serial.println(rcount);

  Serial.println();
  

  delay(250);
}
