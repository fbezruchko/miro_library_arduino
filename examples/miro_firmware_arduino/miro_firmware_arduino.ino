#include "CommLgcSerial.h"

//using namespace miro;

CommLgcSerial parser;

void setup() {
  parser.begin();
}

void loop() {
  parser.handle();
}
