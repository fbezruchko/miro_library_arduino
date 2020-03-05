//Print motors calibration table, stored in eeprom

#include <Miro.h>

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
byte ENCODER_pins[2] = { 2, 3 };
Miro robot(PWM_pins, DIR_pins, ENCODER_pins);

void printWheelTable()
{
  Serial.println(F("MOTORS CALIBRATION TABLE"));
  for (byte w = 0; w < WHEEL_COUNT; w++)
  {
    Serial.print(F("WHEEL "));
    Serial.println(w);
    Serial.print(F("VOLTS"));
    Serial.print(F("   "));
    Serial.print(F("DEG/SEC"));
    Serial.print(F("   "));
    Serial.println(F("BDELAYS"));

    for (byte i = 0; i < WHEEL_TABLE_SIZE; i++)
    {
      Serial.print(robot.chassis.getWheelTableValue(w, VOLTS, i));
      Serial.print(F("   "));
      Serial.print(robot.chassis.getWheelTableValue(w, SPEED, i));
      Serial.print(F("   "));
      Serial.println(robot.chassis.getWheelTableValue(w, BREAKDELAY, i));
    }
    Serial.println();
  }
  return;
}

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  Serial.begin(115200);

  printWheelTable();
}

void loop() {}
