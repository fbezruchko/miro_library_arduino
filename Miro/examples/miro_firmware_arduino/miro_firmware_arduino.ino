#include <Led.h>
#include <Miro.h>

Miro miro;
Led miro_Led_L;

#include "CommLgcSerial.h"


CommLgcSerial parser;

void setup() {
  // put your setup code here, to run once:
  miro.Init();
  miro_Led_L.Init(9);
  miro.attachDevice(&miro_Led_L);
  
  parser.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  parser.handle();
}
