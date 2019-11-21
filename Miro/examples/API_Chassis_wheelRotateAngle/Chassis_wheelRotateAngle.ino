#include <Miro.h>

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
byte ENCODER_pins[2] = { 2, 3 };
Miro robot(PWM_pins, DIR_pins, ENCODER_pins);
unsigned int dcount = 0;
unsigned long count = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  dcount = robot.chassis.wheelGetEncoder(LEFT) - count;
  count = count + dcount;

  Serial.print("Odometry sensor: ");
  Serial.println(dcount);

  delay(2000);
  
  robot.chassis.wheelRotateAng(800, 360, LEFT);
}
