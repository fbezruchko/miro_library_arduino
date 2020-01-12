#include "CommLgcSerial.h"

#include "config.h"
#include <Miro.h>

using namespace miro;

char inputString[RXBUFFERSIZE];      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
byte rx_buffer_pos = 0;

char cmdStrings[CMDBUFFERSIZE][CMDLENGTH];
unsigned char cmdIndex = 0;

extern Miro robot;
extern int mode;

void CommLgcSerial::begin()
{
  Serial.begin(115200);
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

  if (strlen(str) != 0)
  {
    strcpy(cmdStrings[cmdIndex], istr);
    cmdIndex = (cmdIndex+1)/CMDBUFFERSIZE;
    
    istr = strtok(str, " ");

    if (istr[0] == 72) prevCmd();
    else if (istr[0] == 80) nextCmd();
    else if (!strcmp(istr, "help")) help();
    else if (!strcmp(istr, "miroget")) miroget(istr);
    else if (!strcmp(istr, "miroset")) miroset(istr);
    else if (!strcmp(istr, "mirodevtable")) mirodevtable(istr);
    else if (!strcmp(istr, "mirocalibwheel")) mirocalibwheel(istr);
    else if (!strcmp(istr, "mirowheeltable")) mirowheeltable(istr);
    else if (!strcmp(istr, "miromode")) miromode(istr);
    else
    {
      Serial.print(istr);
      Serial.println(F(": not found"));
    }
  }
  Serial.print(F("miro:# "));
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

//========================================== prevCmd
int CommLgcSerial::prevCmd()
{
//  Serial.print('\r');
//  Serial.print(F("miro:# prev cmd:"));
//  cmdIndex = (cmdIndex+1)/CMDBUFFERSIZE;
//  Serial.print(cmdStrings[cmdIndex]);
//  return 0;
}

//========================================== nextCmd
int CommLgcSerial::nextCmd()
{
//  Serial.print('\r');
//  Serial.print(F("miro:# prev cmd:"));
//  cmdIndex = (cmdIndex+1)/CMDBUFFERSIZE;
//  Serial.print(cmdStrings[cmdIndex]);
//  return 0;
}

//========================================== help
int CommLgcSerial::help()
{
  Serial.println(F("help"));
  Serial.println(F("miroget"));
  Serial.println(F("miroset"));
  Serial.println(F("mirodevtable"));
  Serial.println(F("mirowheeltable"));
  Serial.println(F("mirocalibwheel"));
  Serial.println(F("miromode"));
  return 0;
}

//========================================== printWheelTable

void CommLgcSerial::printWheelTable()
{
  for (byte w = 0; w < WHEEL_COUNT; w++)
  {
    Serial.print(F("WHEEL "));
    Serial.println(w);
    Serial.print(F("VOLTS"));
    Serial.print(F(" "));
    Serial.print(F("DEG/SEC"));
    Serial.print(F(" "));
    Serial.println(F("BDELAYS"));

    for (byte i = 0; i < WHEEL_TABLE_SIZE; i++)
    {
      Serial.print(robot.chassis.getWheelTableValue(w, VOLTS, i));
      Serial.print(F(" "));
      Serial.print(robot.chassis.getWheelTableValue(w, SPEED, i));
      Serial.print(F(" "));
      Serial.println(robot.chassis.getWheelTableValue(w, BREAKDELAY, i));
    }
  }
  return;
}

int CommLgcSerial::miroget(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    return -1;
  }

  if (!strcmp(istr, "-s"))
  {
    float path = robot.getPath();
    Serial.print(path);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-fw"))
  {
    Serial.print(MIRO_VERSION);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-v"))
  {
    float linear_speed = 0.0;
    linear_speed = robot.getLinSpeed();
    Serial.print(linear_speed);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-w"))
  {
    float angular_speed = 0.0;
    angular_speed = robot.getAngSpeed();
    Serial.print(angular_speed);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-wR"))
  {
    float angular_speed_radians = 0.0;
    angular_speed_radians = robot.getAngSpeedRad();
    Serial.print(angular_speed_radians);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroget -d: No parameters"));
      return -1;
    }
    int device = atoi(istr);
    if (device < 0 || device > 255) {
      Serial.println(F("miroget -d: Unknown device index"));
      return -1;
    }
    const int count = robot.getDeviceByIndex(device)->getParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroget -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      Serial.println(F("miroget -d: Unknown parameter"));
      return -1;
    }

    if (!strcmp("LED", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN;
      robot.getDeviceByIndex(device)->getParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("USONIC", robot.getDeviceByIndex(device)->getName()))
    {
      int valueN;
      robot.getDeviceByIndex(device)->getParam(paramN, (byte*)&valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("LDR", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN;
      robot.getDeviceByIndex(device)->getParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("LINE", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN;
      robot.getDeviceByIndex(device)->getParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    Serial.println();

    return 0;
  }
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::miroset(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    return -1;
  }

  //==============================================================

  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -r: No parameters"));
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      if (robot.rotate(angular_speed))
      {
        Serial.print(F("Robot angular speed (degrees/s) must be between "));
        Serial.print(robot.getMinAngSpeed());
        Serial.print(F(" and "));
        Serial.print(robot.getMaxAngSpeed());
        Serial.println();
      }
      return 0;
    }

    float angle = atof(istr);

//    Serial.print(F("Angular Speed = "));
//    Serial.print(angular_speed);
//    Serial.print(F(" | Angle = "));
//    Serial.print(angle);
//    Serial.println();
    if (robot.rotateAng(angular_speed, angle, true))
    {
      Serial.print(F("Robot angular speed (degrees/s) must be between "));
      Serial.print(robot.getMinAngSpeed());
      Serial.print(F(" and "));
      Serial.print(robot.getMaxAngSpeed());
      Serial.println();
    }
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-m"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -m: No parameters"));
      return -1;
    }

    float linear_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -m: No 'angular speed' parameter"));
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
//      Serial.print(F("L_speed = "));
//      Serial.print(linear_speed);
//      Serial.print(F(" | A_speed = "));
//      Serial.print(angular_speed);
//      Serial.println();
      if (robot.move(linear_speed, angular_speed))
      {
        Serial.print(F("Robot linear speed (m/s) must be between "));
        Serial.print(robot.getMinLinSpeed());
        Serial.print(F(" and "));
        Serial.print(robot.getMaxLinSpeed());
        Serial.println();
      }
      return 0;
    }
    float distance = atof(istr);

//    Serial.print(F("L_speed = "));
//    Serial.print(linear_speed);
//    Serial.print(F(" | A_speed = "));
//    Serial.print(angular_speed);
//    Serial.print(F(" | distance = "));
//    Serial.print(distance);
//    Serial.println();
    if (robot.moveDist(linear_speed, angular_speed, distance, true))
    {
      Serial.print(F("Robot linear speed (m/s) must be between "));
      Serial.print(robot.getMinLinSpeed());
      Serial.print(F(" and "));
      Serial.print(robot.getMaxLinSpeed());
      Serial.println();
    }
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -d: No parameters"));
      return -1;
    }
    int device = atoi(istr);
    if (device < 0 || device > 255) {
      Serial.println(F("miroset -d: Unknown device index"));
      return -1;
    }
    const int count = robot.getDeviceByIndex(device)->getParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      Serial.println(F("miroset -d: Unknown parameter"));
      return -1;
    }

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("miroset -d: Unknown parameter value"));
      return -1;
    }

    if (!strcmp("LED", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN = atof(istr);
      robot.getDeviceByIndex(device)->setParam(paramN, &valueN);
      //Serial.print((int)valueN);
    }

    if (!strcmp("USONIC", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN = atof(istr);
      robot.getDeviceByIndex(device)->setParam(paramN, &valueN);
      //Serial.print((int)valueN);
    }

    if (!strcmp("SERVO", robot.getDeviceByIndex(device)->getName()))
    {
      byte valueN = atof(istr);
      robot.getDeviceByIndex(device)->setParam(paramN, &valueN);
      //Serial.print((int)valueN);
    }
    
    return 0;
  }
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::mirodevtable(char * str)
{
  for (int i = 0; i < robot.getDeviceCount(); i++)
  {
    Serial.print(i);
    Serial.print(F(" - "));
    Serial.print(robot.getDeviceByIndex(i)->getName());
    Serial.println();
  }
  return 0;
}

int CommLgcSerial::mirocalibwheel(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    Serial.println(F("mirocalibwheel: No 'wheel' parameter"));
    return -1;
  }

  byte wheel = atoi(istr);
  if (wheel < 0 || wheel > robot.chassis.getWheelCount()) {
    Serial.println(F("mirocalibwheel: Wrong wheel index"));
    return -1;
  }
  robot.chassis.wheelCalibrate(wheel);
  this->printWheelTable();
  return 0;
}

int CommLgcSerial::mirowheeltable(char * str)
{
  this->printWheelTable();
  return 0;
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

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::miromode(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    return -1;
  }

  if (!strcmp(istr, "0"))
  {
    mode = 0;
    Serial.println(F("miromode: 0"));
    return 0;
  }

  if (!strcmp(istr, "1"))
  {
    mode = 1;
    Serial.println(F("miromode: 1"));
    return 0;
  }

  if (!strcmp(istr, "2"))
  {
    mode = 2;
    Serial.println(F("miromode: 2"));
    return 0;
  }

  if (!strcmp(istr, "3"))
  {
    mode = 3;
    Serial.println(F("miromode: 3"));
    return 0;
  }

  if (!strcmp(istr, "4"))
  {
    mode = 4;
    Serial.println(F("miromode: 4"));
    return 0;
  }

  if (!strcmp(istr, "5"))
  {
    mode = 5;
    Serial.println(F("miromode: 5"));
    return 0;
  }

  if (!strcmp(istr, "100"))
  {
    mode = 100;
    Serial.println(F("miromode: 100"));
    return 0;
  }
}
