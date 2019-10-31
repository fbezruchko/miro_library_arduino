#include "CommLgcSerial.h"

#include "Miro.h"

using namespace miro;

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

  if (!strcmp(istr, "-fw"))
  {
    Serial.print(F("Version = "));
    Serial.print(MIRO_VERSION);
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

  if (!strcmp(istr, "-rt"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    int angular_speed = atoi(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      Serial.print(F("Rotate speed (PWM) = "));
      Serial.print(angular_speed);
      Serial.println();
      Serial.println(robot.RotatePWM(angular_speed));
      return 0;
    }

    int pwm_time = atoi(istr);

    Serial.print(F("Rotate speed (PWM) = "));
    Serial.print(angular_speed);
    Serial.print(F(" | Rotate time (millis) = "));
    Serial.print(pwm_time);
    Serial.println();
    Serial.println(robot.RotatePWMTime(angular_speed, pwm_time));
    return 0;
  }
  
  //==============================================================
  
  if (!strcmp(istr, "-mt"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    int linear_speed = atoi(istr);
    if (abs(linear_speed) > 255) {
      Serial.println(F("Robot linear speed (PWM) must be between -255 and +255"));
      return -1;
    }

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      Serial.println(F("ERROR"));
      return -1;
    }

    int angular_speed = atoi(istr);
    if (abs(angular_speed) > 255) {
      Serial.println(F("Robot angular speed (PWM) must be between -255 and +255"));
      return -1;
    }

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      Serial.print(F("L_speed = "));
      Serial.print(linear_speed);
      Serial.print(F(" | A_speed = "));
      Serial.print(angular_speed);
      Serial.println();
      Serial.println(robot.MovePWM(linear_speed, angular_speed));
      return 0;
    }
    
    int pwm_time = atoi(istr);
    if (pwm_time < 0) {
      Serial.println(F("Robot moving time (millis) must be between 0 and 16535"));
      return -1;
    }

    Serial.print(F("L_speed = "));
    Serial.print(linear_speed);
    Serial.print(F(" | A_speed = "));
    Serial.print(angular_speed);
    Serial.print(F(" | Moving time = "));
    Serial.print(pwm_time);
    Serial.println();
    Serial.println(robot.MovePWMTime(linear_speed, angular_speed, pwm_time));
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
