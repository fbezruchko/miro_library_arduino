#include <MIROLed.h>
#include <MIROLine.h>
#include <Miro.h>

using namespace miro;

byte PWM_pins[2] = { 10, 9 };
byte DIR_pins[2] = { 8, 7 };
Miro robot(PWM_pins, DIR_pins);

MIROLed robot_Led;
MIROLine robot_Line_M;
MIROLine robot_Line_L;
MIROLine robot_Line_R;

int mode = 0;

#include "CommLgcSerial.h"

CommLgcSerial parser;

void Init_LedSeq()
{
  for (byte i = 0; i < 3; i++)
  {
    robot_Led.On();
    delay(300);
    robot_Led.Off();
    delay(300);
  }
  robot_Led.Off();
}

void setup() {
  // put your setup code here, to run once:
  robot_Led.Init(13);
  robot.attachDevice(&robot_Led);

  robot_Line_M.Init(A1);
  robot_Line_L.Init(A2);
  robot_Line_R.Init(A3);
  robot.attachDevice(&robot_Line_M);
  robot.attachDevice(&robot_Line_L);
  robot.attachDevice(&robot_Line_R);

  pinMode(8,OUTPUT);

  parser.begin();
  Init_LedSeq();
}

void loop() {
  // put your main code here, to run repeatedly:
  parser.handle();

  switch (mode)
  {
    case 0:
      break;
    case 1:
      mode_led_test();
      break;
    case 5:
      mode_motor_test();
      break;
    case 100:
      mode_test();
      break;
    default:
      mode = 0;
      break;
  }
  robot.Sync();
}

void mode_led_test()
{
  Serial.println(F("LED switch on..."));
  robot_Led.On();
  delay(500);
  Serial.println(F("LED switch off..."));
  robot_Led.Off();
  delay(500);
}

void mode_line_test()
{
  Serial.print(F("LINE_L = "));
  Serial.print(robot_Line_L.getValue());
  Serial.print(F(" "));
  Serial.print(F("LINE_R = "));
  Serial.print(robot_Line_R.getValue());
  Serial.print(F(" "));
  Serial.print(F("LINE_M = "));
  Serial.print(robot_Line_M.getValue());
  Serial.println();
  delay(1000);
}

void mode_motor_test()
{
  Serial.print(F("Move forward (PWM value): 127"));
  Serial.println();
  robot.MovePWMTime(127, 0, 1000);
  delay(500);
  
  Serial.print(F("Move forward (PWM value): -127"));
  Serial.println();
  robot.MovePWMTime(-127, 0, 1000);
  delay(500);
}

void mode_test()
{
  mode_led_test();
  mode_motor_test();
}
