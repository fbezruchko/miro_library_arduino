#include <MIROLed.h>
#include <MIROUsonic.h>
#include <MIROLdr.h>
#include <MIROLine.h>
#include <MIROServo.h>
#include <Miro.h>

using namespace miro;

#define MODE_DEFALT 0
#define MODE_LDR_FOLLOW 1

byte PWM_pins[2] = { 5, 6 };
byte DIR_pins[2] = { 4, 7 };
Miro robot(PWM_pins, DIR_pins);

MIROLed robot_Led_L;
MIROLed robot_Led_R;
MIROUsonic robot_Us;
MIROLdr robot_Ldr_L;
MIROLdr robot_Ldr_R;

MIROLine robot_Line_M;
MIROLine robot_Line_L;
MIROLine robot_Line_R;

MIROServo robot_Servo;

int mode = 0;

#include "CommLgcSerial.h"

CommLgcSerial parser;

void Init_LedSeq()
{
  for (byte i = 0; i < 3; i++)
  {
    robot_Led_L.On();
    robot_Led_R.Off();
    delay(300);
    robot_Led_L.Off();
    robot_Led_R.On();
    delay(300);
  }
  robot_Led_L.Off();
  robot_Led_R.Off();
}

void setup() {
  // put your setup code here, to run once:
  robot_Led_L.Init(9);
  robot_Led_R.Init(10);
  robot.attachDevice(&robot_Led_L);
  robot.attachDevice(&robot_Led_R);

  robot_Us.Init(13, 12);
  robot.attachDevice(&robot_Us);

  robot_Ldr_L.Init(A2);
  robot_Ldr_R.Init(A3);
  robot.attachDevice(&robot_Ldr_L);
  robot.attachDevice(&robot_Ldr_R);

  robot_Line_M.Init(A1);
  robot.attachDevice(&robot_Line_M);
  robot_Line_L.Init(A2);
  robot_Line_R.Init(A3);
  robot.attachDevice(&robot_Line_L);
  robot.attachDevice(&robot_Line_R);

  robot_Servo.Init(8, 60, 110);
  robot.attachDevice(&robot_Servo);

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
    case 2:
      mode_ldr_test();
      break;
    case 3:
      mode_us_test();
      break;
    case 4:
      mode_servo_test();
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

void mode_ldr_test()
{
  Serial.print(F("LDR_L = "));
  Serial.print(robot_Ldr_L.getValue());
  Serial.print(F(" "));
  Serial.print(F("LDR_R = "));
  Serial.print(robot_Ldr_R.getValue());
  Serial.println();
  delay(1000);
}

void mode_led_test()
{
  Serial.println(F("LEDs switch on..."));
  robot_Led_L.On();
  robot_Led_R.On();
  delay(500);
  Serial.println(F("LEDs switch off..."));
  robot_Led_L.Off();
  robot_Led_R.Off();
  delay(500);
}

void mode_us_test()
{
  Serial.print(F("US Distance = "));
  Serial.print(robot_Us.getDist(4000));
  Serial.println();
  delay(1000);
}

void mode_servo_test()
{
  Serial.print(F("SERVO [min, max] values = ["));
  Serial.print(robot_Servo.GetMin());
  Serial.print(F(", "));
  Serial.print(robot_Servo.GetMax());
  Serial.print(F("]"));
  Serial.println();

  robot_Servo.Pos(robot_Servo.GetMin());
  delay(100);
  robot_Servo.Pos(robot_Servo.GetMax());
  delay(100);
  robot_Servo.Pos(robot_Servo.GetMin() + (robot_Servo.GetMax() - robot_Servo.GetMin()) / 2);
  delay(800);
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
  Serial.print(F("Move forward (m/s): "));
  Serial.print(robot.chassis.getOptLinSpeed()-0.1, 2);
  Serial.println();
  robot.MoveDist(robot.chassis.getOptLinSpeed()-0.1, 0, 1, false);
  delay(500);
  
  Serial.print(F("Move backward (m/s): "));
  Serial.print(-(robot.chassis.getOptLinSpeed()-0.1), 2);
  Serial.println();
  robot.MoveDist(-(robot.chassis.getOptLinSpeed()-0.1), 0, 1, false);
  delay(500);
}

void mode_test()
{
  mode_led_test();
  mode_ldr_test();
  mode_us_test();
  mode_servo_test();
  mode_motor_test();
}

//void mode_ldr_follow()
//{
//    const float Kp = 0.4;
//    const float LDR_ANG_SPEED = 65;
//    const float LDR_LIN_SPEED = 105;
//    const byte MIN_LDR 190;
//
//    float error = robot_Ldr_L.getValue() - robot_Ldr_R.getValue();
//    float P = Kp * abs(error);
//
//    if ((unsigned int)P > LDR_ANG_SPEED) P = LDR_ANG_SPEED;
//
//    float mean = (robot_Ldr_L.getValue() + robot_Ldr_R.getValue()) / 2.0;
//    if (mean < MIN_LDR)
//    {
//      robot.Move(LDR_LIN_SPEED, angular_speed)
//    }
//    else
//    {
//      robot.Move(0, 0);
//    }
//}
