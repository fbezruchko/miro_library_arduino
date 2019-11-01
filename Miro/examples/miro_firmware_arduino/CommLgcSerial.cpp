#include "CommLgcSerial.h"
#include "Miro.h"

using namespace miro;

#define CHASSIS_W_ENCODERS 1

char inputString[RXBUFFERSIZE];      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
byte rx_buffer_pos = 0;

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

  if (strlen(str) == 0)
  {
    Serial.print(F("Version = "));
    Serial.print(MIRO_VERSION);
    Serial.println();
    Serial.println(F("STATUS: OK"));
    return 0;
  }

  istr = strtok(str, " ");

  if (!strcmp(istr, "miroget")) return (miroget(istr));
  else if (!strcmp(istr, "miroset")) return (miroset(istr));
  else if (!strcmp(istr, "mirodevtable")) return (mirodevtable(istr));
  else if (!strcmp(istr, "mirocalibwheel")) return (mirocalibwheel(istr));
  else if (!strcmp(istr, "mirowheeltable")) return (mirowheeltable(istr));
  else if (!strcmp(istr, "miromode")) return (miromode(istr));
  else
  {
    Serial.println(F("MIRO: Unknown command!"));
    //Serial.println();
  }
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::miroget(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  Serial.print(F("MIRO: miroget running... \""));
  Serial.print(istr);
  Serial.println("\"");
  if (istr == NULL) {
    Serial.println(F("ERROR"));// ;
    return -1;
  }

  if (!strcmp(istr, "-s"))
  {
    float path = robot.getPath();
    Serial.print(F("Path = "));
    Serial.print(path);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-fw"))
  {
    Serial.print(F("Version = "));
    Serial.print(MIRO_VERSION);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-v"))
  {
    float linear_speed = 0.0;
    linear_speed = robot.getLinSpeed();
    Serial.print(F("Linear speed = "));
    Serial.print(linear_speed);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-w"))
  {
    float angular_speed = 0.0;
    angular_speed = robot.getAngSpeed();
    Serial.print(F("Angular speed = "));
    Serial.print(angular_speed);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-wR"))
  {
    float angular_speed_radians = 0.0;
    angular_speed_radians = robot.getAngSpeedRad();
    Serial.print(F("Angular speed in radians = "));
    Serial.print(angular_speed_radians);
    Serial.println();
    return 0;
  }

  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F(">>>NO ARGUMENTS"));
      return -1;
    }
    int device = atoi(istr);
    if (device < 0 || device > 255) {
      Serial.println(F(">>>UNKNOWN DEVICE INDEX"));
      return -1;
    }
    Serial.print(F("Device = "));
    Serial.print(device);
    Serial.println();
    const int count = robot.GetDeviceByIndex(device)->GetParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F(">>>NO PARAMETERS"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      Serial.println(F(">>>UNKNOWN PARAMETER"));
      return -1;
    }

    Serial.print(robot.GetDeviceByIndex(device)->GetName());
    Serial.print(F(" parameter "));
    Serial.print(paramN);
    Serial.print(F(" value = "));
    if (!strcmp("LED", robot.GetDeviceByIndex(device)->GetName()))
    {
      byte valueN;
      robot.GetDeviceByIndex(device)->GetParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("USONIC", robot.GetDeviceByIndex(device)->GetName()))
    {
      int valueN;
      robot.GetDeviceByIndex(device)->GetParam(paramN, (byte*)&valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("LDR", robot.GetDeviceByIndex(device)->GetName()))
    {
      byte valueN;
      robot.GetDeviceByIndex(device)->GetParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("LINE", robot.GetDeviceByIndex(device)->GetName()))
    {
      byte valueN;
      robot.GetDeviceByIndex(device)->GetParam(paramN, &valueN);
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
  Serial.print(F("MIRO: miroset running... \""));
  Serial.print(istr);
  Serial.println(F("\""));
  if (istr == NULL) {
    Serial.println(F("ERROR"));
    return -1;
  }

  //==============================================================

  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      Serial.print(F("Speed = "));
      Serial.print(angular_speed);
      Serial.println();
      if (robot.Rotate(angular_speed))
      {
        Serial.println(F("ERROR"));
        Serial.print(F("Robot angular speed (degrees/s) must be between "));
        Serial.print(robot.chassis.getMinAngSpeed());
        Serial.print(F(" and "));
        Serial.print(robot.chassis.getMaxAngSpeed());
        Serial.println();
      }
      return 0;
    }

    float angle = atof(istr);

    Serial.print(F("Speed = "));
    Serial.print(angular_speed);
    Serial.print(F(" | Angle = "));
    Serial.print(angle);
    Serial.println();
    if (robot.RotateAng(angular_speed, angle, true))
    {
      Serial.println(F("ERROR"));
      Serial.print(F("Robot angular speed (degrees/s) must be between "));
      Serial.print(robot.chassis.getMinAngSpeed());
      Serial.print(F(" and "));
      Serial.print(robot.chassis.getMaxAngSpeed());
      Serial.println();
    }
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-m"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    float linear_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      Serial.print(F("L_speed = "));
      Serial.print(linear_speed);
      Serial.print(F(" | A_speed = "));
      Serial.print(angular_speed);
      Serial.println();
      if (robot.Move(linear_speed, angular_speed))
      {
        Serial.println(F("ERROR"));
        Serial.print(F("Robot linear speed (m/s) must be between "));
        Serial.print(robot.chassis.getMinLinSpeed());
        Serial.print(F(" and "));
        Serial.print(robot.chassis.getMaxLinSpeed());
        Serial.println();
      }
      return 0;
    }
    float distance = atof(istr);

    Serial.print(F("L_speed = "));
    Serial.print(linear_speed);
    Serial.print(F(" | A_speed = "));
    Serial.print(angular_speed);
    Serial.print(F(" | distance = "));
    Serial.print(distance);
    Serial.println();
    if (robot.MoveDist(linear_speed, angular_speed, distance, true))
    {
      Serial.println(F("ERROR"));
      Serial.print(F("Robot linear speed (m/s) must be between "));
      Serial.print(robot.chassis.getMinLinSpeed());
      Serial.print(F(" and "));
      Serial.print(robot.chassis.getMaxLinSpeed());
      Serial.println();
    }
    return 0;
  }

  //==============================================================

  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F(">>>NO ARGUMENTS"));
      return -1;
    }
    int device = atoi(istr);
    if (device < 0 || device > 255) {
      Serial.println(F(">>>UNKNOWN DEVICE INDEX"));
      return -1;
    }
    const int count = robot.GetDeviceByIndex(device)->GetParamCount();

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F(">>>NO PARAMETERS"));
      return -1;
    }
    int paramN = atoi(istr);
    if (paramN <= 0 || paramN > count) {
      Serial.println(F(">>>UNKNOWN PARAMETER"));
      return -1;
    }

    //==============================================================

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F(">>>UNKNOWN PARAMETER VALUE"));
      return -1;
    }

    Serial.print(robot.GetDeviceByIndex(device)->GetName());
    Serial.print(F(" parameter "));
    Serial.print(paramN);
    Serial.print(F(" value = "));
    if (!strcmp("LED", robot.GetDeviceByIndex(device)->GetName()))
    {

      byte valueN = atof(istr);
      robot.GetDeviceByIndex(device)->SetParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("USONIC", robot.GetDeviceByIndex(device)->GetName()))
    {
      byte valueN = atof(istr);
      robot.GetDeviceByIndex(device)->SetParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    if (!strcmp("SERVO", robot.GetDeviceByIndex(device)->GetName()))
    {
      byte valueN = atof(istr);
      robot.GetDeviceByIndex(device)->SetParam(paramN, &valueN);
      Serial.print((int)valueN);
    }

    Serial.println();
    return 0;
  }
}

/************************************************************************/
/************************************************************************/
/************************************************************************/

int CommLgcSerial::mirodevtable(char * str)
{
  Serial.println(F(">>>MIRODEVTABLE CALL"));
  for (int i = 0; i < robot.GetDeviceCount(); i++)
  {
    Serial.print(i);
    Serial.print(F("   -   "));
    Serial.print(robot.GetDeviceByIndex(i)->GetName());
    Serial.println();
  }
  return 0;
}

int CommLgcSerial::mirocalibwheel(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  Serial.print(F("MIRO: mirocalibwheel running... "));
  Serial.println(istr);
  if (istr == NULL) {
    Serial.println(F("ERROR: No 'wheel' parameter."));
    return -1;
  }

  byte wheel = atoi(istr);
  if (wheel < 0 || wheel > robot.chassis.getWheelCount()) {
    Serial.println(F("ERROR: wrong wheel index."));
    return -1;
  }
  robot.chassis.wheelCalibrate(wheel);
  robot.chassis.printWheelTable();
  return 0;
}

int CommLgcSerial::mirowheeltable(char * str)
{
  Serial.println(F("MIRO: mirowheeltable running..."));
  robot.chassis.printWheelTable();
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
  Serial.print(F("MIRO: miromode running... \""));
  Serial.print(istr);
  Serial.println("\"");
  if (istr == NULL) {
    Serial.println(F("ERROR"));// ;
    return -1;
  }

  if (!strcmp(istr, "0"))
  {
    mode = 0;
    Serial.println(F("Switch to mode 0"));
    return 0;
  }

  if (!strcmp(istr, "1"))
  {
    mode = 1;
    Serial.println(F("Switch to mode 1"));
    return 0;
  }

  if (!strcmp(istr, "2"))
  {
    mode = 2;
    Serial.println(F("Switch to mode 2"));
    return 0;
  }

  if (!strcmp(istr, "3"))
  {
    mode = 3;
    Serial.println(F("Switch to mode 3"));
    return 0;
  }

  if (!strcmp(istr, "4"))
  {
    mode = 4;
    Serial.println(F("Switch to mode 4"));
    return 0;
  }

  if (!strcmp(istr, "5"))
  {
    mode = 5;
    Serial.println(F("Switch to mode 5"));
    return 0;
  }

  if (!strcmp(istr, "100"))
  {
    mode = 100;
    Serial.println(F("Switch to mode 100. Full test!"));
    return 0;
  }
}
