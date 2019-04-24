#include <Miro.h>

Miro robot;
unsigned int dcount = 0;
unsigned long count = 0;


void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  robot.Init();
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  dcount = robot.chassis.wheelGetTachom(LEFT) - count;
  count = count + dcount;

  Serial.print("Odometry sensor: ");
  Serial.println(dcount);

  delay(2000);
  
  robot.chassis.wheelRotateAng(800, 360, LEFT);
}
