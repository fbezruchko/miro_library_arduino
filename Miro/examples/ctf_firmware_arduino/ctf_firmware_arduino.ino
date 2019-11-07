#include "config.h"
#include <MIROLed.h>
#include <MIROLine.h>
#include <Miro.h>

//using namespace miro;

byte PWM_pins[2] = { 10, 9 };
byte DIR_pins[2] = { 8, 7 };
Miro robot(PWM_pins, DIR_pins);

MIROLed robot_Led;

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

  parser.begin();
  Init_LedSeq();
}

void loop() {
  // put your main code here, to run repeatedly:
  parser.handle();
  robot.Sync();
}
