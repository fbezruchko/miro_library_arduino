#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>

#include "CommLgcSerial.h"

#include "MiroDevices.h"
#include "Miro.h"

Miro robot;
MiroDevices devices;

#define PROGMEM_CMD_NAMES 9
const char s_help[] PROGMEM = "help";
const char s_get[] PROGMEM = "get";
const char s_set[] PROGMEM = "set";
const char s_devTable[] PROGMEM = "devtable";
const char s_calibWheel[] PROGMEM = "calibwheel";
const char s_wheelTable[] PROGMEM = "wheeltable";
const char s_attach[] PROGMEM = "attach";
const char s_detach[] PROGMEM = "detach";
const char s_devIDs[] PROGMEM = "devids";

const char *const cmd_names[PROGMEM_CMD_NAMES] PROGMEM =
    {
        s_help,
        s_get,
        s_set,
        s_devTable,
        s_calibWheel,
        s_wheelTable,
        s_attach,
        s_detach,
        s_devIDs};

#define CMD_HELP 0
#define CMD_GET 1
#define CMD_SET 2
#define CMD_DEVTABLE 3
#define CMD_CALIBWHEEL 4
#define CMD_WHEELTABLE 5
#define CMD_ATTACH 6
#define CMD_DETACH 7
#define CMD_DEVIDS 8

//=======================================

#define PROGMEM_ERROR_MSGS 4
const char s_error0[] PROGMEM = "No parameters";
const char s_error1[] PROGMEM = "Unknown parameter";
const char s_error2[] PROGMEM = "Wrong parameter value";
const char s_error3[] PROGMEM = "Unrecognized parameter type";

const char *const error_msgs[PROGMEM_ERROR_MSGS] PROGMEM =
    {
        s_error0,
        s_error1,
        s_error2,
        s_error3};

char inputString[RXBUFFERSIZE]; // a String to hold incoming data
bool stringComplete = false;    // whether the string is complete
uint8_t rxBufferPos = 0;

char outputString[TXBUFFERSIZE];

float strtof(const char *str)
{
  int8_t inc;
  float result = 0.0;
  char *c_tmp;
  c_tmp = strchr(str, '.');
  if (c_tmp != NULL)
  {
    c_tmp++;
    inc = -1;
    while (*c_tmp != 0 && inc > -9)
    {
      result += (*c_tmp - '0') * pow(10.0, inc);
      c_tmp++;
      inc--;
    }
    inc = 0;
    c_tmp = strchr(str, '.');
    c_tmp--;
    do
    {
      result += (*c_tmp - '0') * pow(10.0, inc);
      c_tmp--;
      inc++;
    } while (c_tmp >= str);
  }
  return result;
}

// void print_P(const char *P_string)
// {
//   strcpy_P(outputString, P_string);
//   Serial.print(outputString);
// }

void println_P(const char *P_string)
{
  strcpy_P(outputString, P_string);
  Serial.println(outputString);
}

void CommLgcSerial::begin()
{
  Serial.begin(115200);
}

void CommLgcSerial::handle()
{
  if (stringComplete)
  {
    CommLgcSerial::parse(inputString);
    memset(inputString, 0, RXBUFFERSIZE);
    stringComplete = false;
    rxBufferPos = 0;
  }

  robot.sync();
}

// int fullRAM()
// {
//   extern int __data_start;
//   return (RAMEND + 1) - (__data_start - 1);
// }

// int dataRAM()
// {
//   extern char *__malloc_heap_start;
//   extern int __data_start;
//   return (int)(__malloc_heap_start - (__data_start - 1));
// }

// int heapRAM()
// {
//   extern char *__malloc_heap_start, *__brkval;
//   return (__brkval == 0 ? 0 : ((int)__brkval - (int)__malloc_heap_start));
// }

int stackRAM()
{
  int v;
  return (RAMEND + 1) - (int)&v;
}

int freeRAM()
{
  extern char __heap_start, *__brkval;
  int v;
  return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

int CommLgcSerial::parse(char *str)
{
  char *istr;

  if (strlen(str) != 0)
  {
    Serial.println();

    istr = strtok(str, " ");

    if (!strcmp_P(istr, cmd_names[0]))
      help();
    else if (!strcmp_P(istr, cmd_names[1]))
      get();
    else if (!strcmp_P(istr, cmd_names[2]))
      set();
    else if (!strcmp_P(istr, cmd_names[3]))
      devTable();
    else if (!strcmp_P(istr, cmd_names[4]))
      calibWheel();
    else if (!strcmp_P(istr, cmd_names[5]))
      wheelTable();
    else if (!strcmp_P(istr, cmd_names[6]))
      attach();
    else if (!strcmp_P(istr, cmd_names[7]))
      detach();
    else if (!strcmp_P(istr, cmd_names[8]))
      devIDs();
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
  //  Serial.print(F("/"));
  //  Serial.print(dataRAM());
  //  Serial.print(F("/"));
  //  Serial.print(heapRAM());
  //  Serial.print(F("/"));
  //  Serial.print(stackRAM());
  //  Serial.print(F("/"));
  //  Serial.print(fullRAM());
  Serial.print(F(")#: "));

  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

//========================================== help
void CommLgcSerial::help()
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
  return;
}

//========================================== printWheelTable

void CommLgcSerial::wheelTable()
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

int CommLgcSerial::get()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
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

  if (!strcmp(istr, "-f"))
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

  if (!strcmp(istr, "-W"))
  {
    float angular_speed_radians = 0.0;
    angular_speed_radians = robot.getAngSpeedRad();
    Serial.print(angular_speed_radians);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-i"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("No parameters"));
      return -1;
    }
    uint8_t i;
    for (i = 0; i < robot.getDeviceCount(); i++)
    {
      if (!strcmp(istr, (const char*)(robot.getDeviceByIndex(i))->getParam(0)))
        break;
    }
    if ((i + 1) > robot.getDeviceCount())
    {
      return -1;
    }
    Serial.println(i);
  }

  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
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
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN < 0 || paramN > count)
    {
      println_P(error_msgs[1]); //Serial.println(F("miroget -d: Unknown parameter"));
      return -1;
    }

    uint8_t paramType = robot.getDeviceByIndex(dev_i)->getParamType(paramN);
    uint8_t *valueN;
    valueN = robot.getDeviceByIndex(dev_i)->getParam(paramN);
    switch (paramType)
    {
    case PTYPE_BOOL:
      Serial.print(*((bool *)valueN));
      break;
    case PTYPE_CHAR:
      Serial.print(*((char *)valueN));
      break;
    case PTYPE_CSTRING:
      Serial.print((char *)valueN);
      break;
    case PTYPE_UINT8:
      Serial.print(*valueN);
      break;
    case PTYPE_INT8:
      Serial.print(*((int8_t *)valueN));
      break;
    case PTYPE_UINT16:
      Serial.print(*((uint16_t *)valueN));
      break;
    case PTYPE_INT16:
      Serial.print(*((int16_t *)valueN));
      break;
    case PTYPE_UINT32:
      Serial.print(*((uint32_t *)valueN));
      break;
    case PTYPE_INT32:
      Serial.print(*((int32_t *)valueN));
      break;
    case PTYPE_FLOAT:
      Serial.print(*((float *)valueN));
      break;
    case PTYPE_VOID:
    default:
      println_P(error_msgs[3]);
      break;
    }

    Serial.println();

    return 0;
  }
  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::set()
{
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
    println_P(error_msgs[0]); //Serial.println(F("miroget -d: No parameters"));
    return -1;
  }

  //==============================================================

  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("miroset -r: No parameters"));
      return -1;
    }

    float angular_speed = strtof(istr);

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

    float angle = strtof(istr);

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
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("miroset -m: No parameters"));
      return -1;
    }

    float linear_speed = strtof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("miroset -m: No parameters"));
      return -1;
    }

    float angular_speed = strtof(istr);

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
    float distance = strtof(istr);

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
    if (istr == NULL)
    {
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
    if (istr == NULL)
    {
      println_P(error_msgs[0]); //Serial.println(F("miroset -d: No parameters"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count)
    {
      println_P(error_msgs[1]); //Serial.println(F("miroset -d: Unknown parameter"));
      return -1;
    }

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      println_P(error_msgs[2]); //Serial.println(F("miroset -d: Wrong parameter value"));
      return -1;
    }

    uint8_t pLen = strlen(istr);
    char *pEnd;

    long int valueI = strtol(istr, &pEnd, 0);
    if (pEnd == (istr + pLen))
    {
      robot.getDeviceByIndex(dev_i)->setParam(paramN, (uint8_t *)(&valueI));
      //Serial.println(valueI);
    }
    else
    {
      float valueF = strtof(istr);
      robot.getDeviceByIndex(dev_i)->setParam(paramN, (uint8_t *)(&valueF));
      //Serial.println(valueF);
    }

    return 0;
  }
  return 0;
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

void CommLgcSerial::devTable()
{
  Device *d;
  for (int i = 0; i < robot.getDeviceCount(); i++)
  {
    d = robot.getDeviceByIndex(i);
    Serial.print(i);
    Serial.print(F(" - "));
    Serial.print((char *)d->getParam(0));
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
  return;
}

int CommLgcSerial::calibWheel()
{
#if defined(CHASSIS_CALIBRATION_ON)
  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
    println_P(error_msgs[0]); //Serial.println(F("mirocalibwheel: No parameters"));
    return -1;
  }

  int8_t wheel = atoi(istr);
  if (wheel < 0 || wheel > robot.chassis.getWheelCount())
  {
    println_P(error_msgs[2]); //Serial.println(F("mirocalibwheel: Wrong parameter value"));
    return -1;
  }
  robot.chassis.wheelCalibrate(wheel);
  this->wheelTable();
#else
  Serial.println(F("This function is disabled in your firmware"));
#endif
  return 0;
}

int CommLgcSerial::attach()
{
  int id = -1;
  uint8_t pins_count;
  bool default_pins = false;

  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
    println_P(error_msgs[0]); //Serial.println(F("No parameters"));
    return -1;
  }

  //==============================================================

  if ((istr[0] >= 0x30) && (istr[0] <= 0x39))
  {
    id = atoi(istr);
  }
  else
  {
    id = devices.getDeviceID(istr);
  }

  //  if (id < 0)
  //  {
  //    println_P(error_msgs[1]); //Serial.println(F("Unknown parameter"));
  //    return -1;
  //  }
  pins_count = devices.getDevicePinsCount(id);
  uint8_t pins[pins_count];
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
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
      if ((istr == NULL) && ((pins_count - i) > 1))
      {
        println_P(error_msgs[0]); //Serial.println(F("No parameters"));
        return -1;
      }
    }
  }

  Device *d;
  if (default_pins)
    d = devices.createDevice(id);
  else
    d = devices.createDevice(id, pins);

  robot.attachDevice(d);

  return 0;
}

int CommLgcSerial::detach()
{
  int dev_i;

  char *istr;
  istr = strtok(NULL, " ");
  if (istr == NULL)
  {
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

  Device *d = robot.getDeviceByIndex(dev_i);
  robot.dettachDevice(dev_i);
  devices.destroyDevice(d);

  return 0;
}

int getDevIndex(char *devName)
{
  uint8_t i;
  for (i = 0; i < robot.getDeviceCount(); i++)
  {
    if (!strcmp(devName, (char *)(robot.getDeviceByIndex(i))->getParam(0)))
      break;
  }
  if ((i + 1) > robot.getDeviceCount())
  {
    return -1;
  }

  return i;
}

void CommLgcSerial::devIDs()
{
  uint8_t count = devices.getDeviceIDsCount();
  Serial.println(F("ID - NAME - PINS COUNT - DEFAULT PINS"));
  for (uint8_t i = 0; i < count; i++)
  {
    Serial.print(i);
    Serial.print(F(" - "));
    Serial.print(devices.getDeviceName(i));
    Serial.print(F(" - "));
    Serial.print(devices.getDevicePinsCount(i));
    Serial.print(F(" - "));
    for (uint8_t j = 0; j < devices.getDevicePinsCount(i); j++)
    {
      Serial.print(devices.getDeviceDefaultPin(i, j));
      Serial.print(F("("));
      Serial.print(devices.getDevicePinName(i, j));
      Serial.print(F("), "));
    }
    Serial.println();
  }
  return;
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple uint8_ts of data may be available.
*/
void serialEvent()
{
  while (Serial.available())
  {
    // get the new uint8_t:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[rxBufferPos] = inChar;
    rxBufferPos++;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n')
    {
      stringComplete = true;
      inputString[rxBufferPos - 1] = 0;
      if (inputString[rxBufferPos - 2] == '\r')
        inputString[rxBufferPos - 2] = 0;
    }
  }
}