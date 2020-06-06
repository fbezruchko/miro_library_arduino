#include <Arduino.h>
#include "CommLgcSerial.h"

CommLgcSerial parser;

void setup()
{
  parser.begin();
}

void loop()
{
  parser.handle();
}