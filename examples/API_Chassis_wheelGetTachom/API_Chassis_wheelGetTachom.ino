//Rotate wheels and control odometry sensors work in serial terminal

#include <Miro.h>

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
byte ENCODER_pins[2] = { 2, 3 };
Miro robot(PWM_pins, DIR_pins, ENCODER_pins);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long lcount = robot.chassis.wheelGetEncoder(LEFT);
  unsigned long rcount = robot.chassis.wheelGetEncoder(RIGHT);

  Serial.print("Left odometry sensor: ");
  Serial.println(lcount);
  
  Serial.print("Rigth odometry sensor: ");
  Serial.println(rcount);

  Serial.println();
  

  delay(250);
}
