//Print motors calibration table, stored in eeprom

#include <Miro.h>

Miro robot;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(9600);
  robot.Init();

  robot.chassis.printWheelTable();
}

void loop() {}
