#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "CommLgcSerial.h"

#include <MIRODevices.h>
#include <Miro.h>

//using namespace miro;

Miro robot;
MIRODevices devices;

#define PROGMEM_CMD_NAMES 10
const char s_help[] PROGMEM = "help";
const char s_miroget[] PROGMEM = "miroget";
const char s_miroset[] PROGMEM = "miroset";
const char s_mirodevtable[] PROGMEM = "mirodevtable";
const char s_mirocalibwheel[] PROGMEM = "mirocalibwheel";
const char s_mirowheeltable[] PROGMEM = "mirowheeltable";
const char s_miroattach[] PROGMEM = "miroattach";
const char s_mirodetach[] PROGMEM = "mirodetach";
const char s_mirogetdevname[] PROGMEM = "mirogetdevname";
const char s_mirogetdevix[] PROGMEM = "mirogetdevix";

const char * const cmd_names[PROGMEM_CMD_NAMES] PROGMEM =
{
  s_help,
  s_miroget,
  s_miroset,
  s_mirodevtable,
  s_mirocalibwheel,
  s_mirowheeltable,
  s_miroattach,
  s_mirodetach,
  s_mirogetdevname,
  s_mirogetdevix
};

#define CMD_HELP 0
#define CMD_MIROGET 1
#define CMD_MIROSET 2
#define CMD_MIRODEVTABLE 3
#define CMD_MIROCALIBWHEEL 4
#define CMD_MIROWHEELTABLE 5
#define CMD_MIROATTACH 6
#define CMD_MIRODETACH 7
#define CMD_MIROGETDEVNAME 8
#define CMD_MIROGETDEVIX 9

//=======================================

#define PROGMEM_ERROR_MSGS 3
const char s_error0[] PROGMEM = "No parameters";
const char s_error1[] PROGMEM = "Unknown parameter";
const char s_error2[] PROGMEM = "Wrong parameter value";

const char* const error_msgs[PROGMEM_ERROR_MSGS] PROGMEM =
{
  s_error0,
  s_error1,
  s_error2
};

char inputString[RXBUFFERSIZE];      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
uint8_t rx_buffer_pos = 0;

char outputString[TXBUFFERSIZE];

void print_P(const char* P_string)
{
  strcpy_P(outputString, P_string);
  Serial.print(outputString);
}

void println_P(const char* P_string)
{
  strcpy_P(outputString, P_string);
  Serial.println(outputString);
}

void CommLgcSerial::begin()
{
  Serial.begin(9600);
  //uint8_t pin = 13;
  //Device* d = DEVICE_FABRIC[0](&pin, 1);
  //robot.attachDevice(d);
}

void CommLgcSerial::handle()
{
  if (stringComplete) {
    CommLgcSerial::parse(inputString);
    memset(inputString, 0, RXBUFFERSIZE);
    stringComplete = false;
    rx_buffer_pos = 0;
  }

  robot.Sync();
}

int fullRAM()
{
  extern int __data_start;
  return (RAMEND + 1) - (__data_start - 1);
}

int dataRAM()
{
  extern char *__malloc_heap_start;
  extern int __data_start;
  return (int)(__malloc_heap_start - (__data_start - 1));
}

int heapRAM()
{
  extern char *__malloc_heap_start, *__brkval;
  return (__brkval == 0 ? 0 : ((int)__brkval - (int)__malloc_heap_start));
}

int stackRAM()
{
  int v;
  return (RAMEND + 1) - (int) &v;
}

int freeRAM()
{
  extern char __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int CommLgcSerial::parse(char *str)
{
  char *istr;

  if (strlen(str) != 0)
  {
    Serial.println();

    istr = strtok(str, " ");

    if (!strcmp_P(istr, cmd_names[0])) help();
    else if (!strcmp_P(istr, cmd_names[1])) miroget();
    else if (!strcmp_P(istr, cmd_names[2])) miroset();
    else if (!strcmp_P(istr, cmd_names[3])) mirodevtable();
    else if (!strcmp_P(istr, cmd_names[4])) mirocalibwheel();
    else if (!strcmp_P(istr, cmd_names[5])) mirowheeltable();
    else if (!strcmp_P(istr, cmd_names[6])) miroattach();
    else if (!strcmp_P(istr, cmd_names[7])) mirodetach();
    else if (!strcmp_P(istr, cmd_names[8])) mirogetdevname();
    else if (!strcmp_P(istr, cmd_names[9])) mirogetdevix();
    else
    {
      Serial.print(istr);
      Serial.println(F(": not found"));
    }
  }
  else
  {
    Serial.println();
  }

  Serial.print(F("miro("));
  Serial.print(freeRAM());
  Serial.print(F("/"));
  Serial.print(dataRAM());
  Serial.print(F("/"));
  Serial.print(heapRAM());
  Serial.print(F("/"));
  Serial.print(stackRAM());
  Serial.print(F("/"));
  Serial.print(fullRAM());
  Serial.print(F(")#: "));

  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

//========================================== help
int CommLgcSerial::help()
{
  println_P(cmd_names[0]);
  println_P(cmd_names[1]);
  println_P(cmd_names[2]);
  println_P(cmd_names[3]);
  println_P(cmd_names[4]);
  println_P(cmd_names[5]);
  println_P(cmd_names[6]);
  println_P(cmd_names[7]);
  println_P(cmd_names[8]);
  println_P(cmd_names[9]);
  return 0;
}

//========================================== printWheelTable

void CommLgcSerial::printWheelTable()
{
  for (uint8_t w = 0; w < WHEEL_COUNT; w++)
  {
    Serial.print(F("WHEEL "));
    Serial.println(w);
    Serial.print(F("VOLTS"));
    Serial.print(F(" "));
    Serial.print(F("DEG/SEC"));
    Serial.print(F(" "));
    Serial.println(F("BDELAYS"));

    for (uint8_t i = 0; i < WHEEL_TABLE_SIZE; i++)
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

int CommLgcSerial::miroget()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
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
      println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
      return -1;
    }
    int dev_i = atoi(istr);
    if ((dev_i < 0) || (dev_i > (robot.getDeviceCount() - 1)))
    {
      println_P(error_msgs[2]); //Serial.println(F("miroget -d: Wrong parameter value"));
      return -1;
    }
    const int count = robot.getDeviceByIndex(dev_i)->getParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      println_P(error_msgs[1]); //Serial.println(F("miroget -d: Unknown parameter"));
      return -1;
    }

    uint8_t valueN;
    robot.getDeviceByIndex(dev_i)->getParam(paramN, &valueN);
    Serial.print((int)valueN);

    Serial.println();

    return 0;
  }
  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::miroset()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
    return -1;
  }

  //==============================================================

  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      println_P(error_msgs[0]); //Serial.println(F("miroset -r: No parameters"));
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
      println_P(error_msgs[0]); //Serial.println(F("miroset -m: No parameters"));
      return -1;
    }

    float linear_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      println_P(error_msgs[0]); //Serial.println(F("miroset -m: No parameters"));
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
      println_P(error_msgs[0]); //Serial.println(F("miroset -d: No parameters"));
      return -1;
    }
    int dev_i = atoi(istr);
    if ((dev_i < 0) || (dev_i > (robot.getDeviceCount() - 1)))
    {
      println_P(error_msgs[2]); //Serial.println(F("miroset -d: Wrong parameter value"));
      return -1;
    }
    const int count = robot.getDeviceByIndex(dev_i)->getParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      println_P(error_msgs[0]); //Serial.println(F("miroset -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      println_P(error_msgs[1]); //Serial.println(F("miroset -d: Unknown parameter"));
      return -1;
    }

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      println_P(error_msgs[2]); //Serial.println(F("miroset -d: Wrong parameter value"));
      return -1;
    }

    uint8_t valueN = atof(istr);
    robot.getDeviceByIndex(dev_i)->setParam(paramN, &valueN);
    //Serial.print((int)valueN);

    return 0;
  }
  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::mirodevtable()
{
  Device *d;
  for (int i = 0; i < robot.getDeviceCount(); i++)
  {
    d = robot.getDeviceByIndex(i);
    Serial.print(i);
    Serial.print(F(" - "));
    Serial.print(d->getName());
    Serial.print(F(" - "));
    for (uint8_t j = 0; j < d->getPinsCount(); j++)
    {
      Serial.print(d->getPinNum(j));
      Serial.print('(');
      Serial.print(d->getPinType(j));
      Serial.print(F("), "));
    }
    Serial.println();
  }
  return 0;
}

int CommLgcSerial::mirocalibwheel()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("mirocalibwheel: No parameters"));
    return -1;
  }

  int8_t wheel = atoi(istr);
  if (wheel < 0 || wheel > robot.chassis.getWheelCount()) {
    println_P(error_msgs[2]); //Serial.println(F("mirocalibwheel: Wrong parameter value"));
    return -1;
  }
  robot.chassis.wheelCalibrate(wheel);
  this->printWheelTable();
  return 0;
}

int CommLgcSerial::mirowheeltable()
{
  this->printWheelTable();
  return 0;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple uint8_ts of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new uint8_t:
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

int CommLgcSerial::miroattach()
{
  int id = -1;
  uint8_t pins_count;
  bool default_pins = false;

  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("No parameters"));
    return -1;
  }

  //==============================================================

  id = devices.getDeviceId(istr);
  //  if (id < 0)
  //  {
  //    println_P(error_msgs[1]); //Serial.println(F("Unknown parameter"));
  //    return -1;
  //  }
  pins_count = devices.getDevicePinsCount(id);
  uint8_t pins[pins_count];
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    default_pins = true;
  }
  else
  {
    for (int i = 0; i < pins_count; i++)
    {
      pins[i] = atoi(istr);
      if (pins[i] <= 0)
      {
        println_P(error_msgs[2]); //Serial.println(F("Wrong parameter value"));
        return -1;
      }
      istr = strtok(NULL, " ");
      if ((istr == NULL) && ((pins_count - i) > 1)) {
        println_P(error_msgs[0]); //Serial.println(F("No parameters"));
        return -1;
      }
    }
  }

  Device* d;
  if (default_pins)
    d = devices.CreateDevice(id);
  else
    d = devices.CreateDevice(id, pins);

  robot.attachDevice(d);

  return 0;
}

int CommLgcSerial::mirodetach()
{
  int dev_i;

  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("No parameters"));
    return -1;
  }

  //==============================================================

  dev_i = atoi(istr);
  if ((dev_i == 0) && (strcmp(istr, "0")))
  {
    println_P(error_msgs[2]); //Serial.println(F("Wrong parameter value"));
    return -1;
  }
  if ((dev_i < 0) || (dev_i > (robot.getDeviceCount() - 1)))
  {
    println_P(error_msgs[2]); //Serial.println(F("Wrong parameter value"));
    return -1;
  }

  Device* d = robot.getDeviceByIndex(dev_i);
  robot.dettachDevice(dev_i);
  devices.DestroyDevice(d);

  return 0;
}


int CommLgcSerial::mirogetdevname()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("No parameters"));
    return -1;
  }

  int dev_i = atoi(istr);
  if (((dev_i == 0) && (strcmp(istr, "0"))) || (dev_i > (robot.getDeviceCount() - 1)) || (dev_i < 0))
  {
    println_P(error_msgs[2]); //Serial.println(F("Wrong parameter value"));
    return -1;
  }

  Device* d = robot.getDeviceByIndex(dev_i);
  Serial.println(d->getName());

  return 0;
}

int CommLgcSerial::mirogetdevix()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL) {
    println_P(error_msgs[0]); //Serial.println(F("No parameters"));
    return -1;
  }

  uint8_t i;
  for (i = 0; i < robot.getDeviceCount(); i++)
  {
    if (!strcmp(istr, (robot.getDeviceByIndex(i))->getName())) break;
  }

  if ((i + 1) > robot.getDeviceCount())
  {
    println_P(error_msgs[2]); //Serial.println(F("Wrong parameter value"));
    return -1;
  }

  Serial.println(i);

  return 0;
}
