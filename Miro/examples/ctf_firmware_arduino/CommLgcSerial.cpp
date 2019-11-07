#include "CommLgcSerial.h"

#include "config.h"
#include <Miro.h>

//using namespace miro;

char inputString[RXBUFFERSIZE];      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
byte rx_buffer_pos = 0;

extern Miro robot;

void CommLgcSerial::begin()
{
  Serial.begin(9600);
}

void CommLgcSerial::handle()
{
  if (stringComplete) {
    CommLgcSerial::parse(inputString);
    memset(inputString, 0, RXBUFFERSIZE);
    stringComplete = false;
    rx_buffer_pos = 0;
  }
}

int CommLgcSerial::parse(char *str)
{
  char *istr;

  if (strlen(str) == 0)
  {
    Serial.print(F("Version = "));
    Serial.print(MIRO_VERSION);
    Serial.println();
    Serial.println(F("STATUS: OK"));
    return 0;
  }

  istr = strtok(str, " ");

  if (!strcmp(istr, "miromove")) return (miromove(istr));
  else
  {
    Serial.println(F("MIRO: Unknown command!"));
    //Serial.println();
  }
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::miromove(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  Serial.print(F("MIRO: miromove running... \""));
  Serial.print(istr);
  Serial.println(F("\""));
  if (istr == NULL) {
    Serial.println(F("ERROR"));
    return -1;
  }

  //==============================================================

  if (!strcmp(istr, "-f"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      return 0;
    }

    int pwm_time = atoi(istr);
    if (pwm_time < 0) {
      Serial.println(F("Robot moving time (millis) must be between 0 and 16535"));
      return -1;
    }

    Serial.print(F("Moving time = "));
    Serial.print(pwm_time);
    Serial.print(F(" ... "));
    robot.movePWMTime(192, 0, pwm_time);
    Serial.println(F("DONE"));
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-b"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      return 0;
    }

    int pwm_time = atoi(istr);
    if (pwm_time < 0) {
      Serial.println(F("Robot moving time (millis) must be between 0 and 16535"));
      return -1;
    }

    Serial.print(F("Moving time = "));
    Serial.print(pwm_time);
    Serial.print(F(" ... "));
    robot.movePWMTime(-192, 0, pwm_time);
    Serial.println(F("DONE"));
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      return 0;
    }

    int pwm_time = atoi(istr);
    if (pwm_time < 0) {
      Serial.println(F("Robot rotate time (millis) must be between 0 and 16535"));
      return -1;
    }

    Serial.print(F("Rotate time = "));
    Serial.print(pwm_time);
    Serial.print(F(" ... "));
    robot.rotatePWMTime(-192, pwm_time);
    Serial.println(F("DONE"));
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-l"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      return 0;
    }

    int pwm_time = atoi(istr);
    if (pwm_time < 0) {
      Serial.println(F("Robot rotate time (millis) must be between 0 and 16535"));
      return -1;
    }

    Serial.print(F("Rotate time = "));
    Serial.print(pwm_time);
    Serial.print(F(" ... "));
    robot.rotatePWMTime(192, pwm_time);
    Serial.println(F("DONE"));
    return 0;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[rx_buffer_pos] = inChar;
    rx_buffer_pos++;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
      inputString[rx_buffer_pos - 1] = 0;
      if (inputString[rx_buffer_pos - 2] == '\r') inputString[rx_buffer_pos - 2] = 0;
    }
  }
}
