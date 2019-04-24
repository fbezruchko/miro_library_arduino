#include "CommLgcSerial.h"

#include <string.h>
//#include <stdio.h>
#include <stdlib.h>
#include "Miro.h"

char inputString[RXBUFFERSIZE];      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
unsigned int rx_buffer_pos = 0;

extern Miro miro;

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

  istr = strtok(str, " ");
  Serial.print("SHELL COMMAND: \"");
  Serial.print(istr);
  Serial.println("\"");

  if (!strcmp(istr, "miroget")) return (miroget(istr));
  else if (!strcmp(istr, "miroset")) return (miroset(istr));
  else if (!strcmp(istr, "mirodevtable")) return (mirodevtable(istr));
  else Serial.println("Unknown shell command!");
}

int CommLgcSerial::miroget(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  Serial.print("MIROGET CALL: \"");
  Serial.print(istr);
  Serial.println("\"");
  if (istr == NULL) {
    /*printf("ERROR \n");*/ Serial.println("ERROR");
    return -1;
  }
  if (!strcmp(istr, "-s"))
  {
    float path = 0.0;
    /*printf("Path = %f \n", path);*/
    Serial.print("Path = ");
    Serial.print(path);
    Serial.println();
    return 0;
  }
  if (!strcmp(istr, "-fw"))
  {
    return 0;
  }
  if (!strcmp(istr, "-v"))
  {
    float linear_speed = 0.0;
    //path = Miro::getLinSpeed();
    //printf("Linear speed = %f \n", linear_speed);
    Serial.print("Linear speed = ");
    Serial.print(linear_speed);
    Serial.println();
    return 0;
  }
  if (!strcmp(istr, "-w"))
  {
    float angular_speed = 0.0;
    //path = Miro::getAngSpeed();
    //printf("Angular speed = %f \n", angular_speed);
    Serial.print("Angular speed = ");
    Serial.print(angular_speed);
    Serial.println();
    return 0;
  }
  if (!strcmp(istr, "-wR"))
  {
    float angular_speed_radians = 0.0;
    //path = Miro::getAngSpeedRad();
    //printf("Angular speed in radians = %f \n", angular_speed_radians);
    Serial.print("Angular speed in radians = ");
    Serial.print(angular_speed_radians);
    Serial.println();
    return 0;
  }
  if (!strcmp(istr, "-d"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");
      return -1;
    }

    int device = atoi(istr);
    if (device < 0 || device > 255) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");
      return -1;
    }

    //printf("Device = %d \n", device);
    Serial.print("Device = ");
    Serial.print(device);
    Serial.println();
    // ����� ������� miroget -d
    return 0;
  }
  else {
    /*printf("ERROR \n");*/ Serial.println("ERROR");
    return -1;
  }
}

int CommLgcSerial::miroset(char * str)
{
  char *istr;
  istr = strtok(NULL, " ");
  Serial.print("MIROSET CALL: \"");
  Serial.print(istr);
  Serial.println("\"");
  if (istr == NULL) {
    /*printf("ERROR \n");*/ Serial.println("ERROR");
    return -1;
  }
  if (!strcmp(istr, "-r"))
  {
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");  // ������ �������� ������ �������
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      //printf("Speed = %f \n", angular_speed); // ������ ���� ��������
      Serial.print("Speed = ");
      Serial.print(angular_speed);
      Serial.println();
//      miro.Rotate(angular_speed);
      return 0;
    }
    float angle = atof(str);

    //printf("Speed = %f | Angle = %f \n", angular_speed, angle); // ������� ��� ���������
    Serial.print("Speed = ");
    Serial.print(angular_speed);
    Serial.print(" | Angle = ");
    Serial.print(angle);
    Serial.println();
    miro.RotateAng(angular_speed, angle);
    return 0;
  }
  if (!strcmp(istr, "-m"))
  {
    //printf("miroset -m \n");
    Serial.println("miroset -m ");

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");  // ������ �������� ������ �������
      return -1;
    }

    float linear_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");  // ������ �������� ������ �������
      return -1;
    }

    float angular_speed = atof(istr);

    istr = strtok(NULL, " ");
    if (istr == NULL)
    {
      //printf("L_speed = %f | A_speed = %f \n", linear_speed, angular_speed); // ������� 2 ���������
      Serial.print("L_speed = ");
      Serial.print(linear_speed);
      Serial.print(" | A_speed = ");
      Serial.print(angular_speed);
      Serial.println();
//      miro.Move(linear_speed, angular_speed);
      return 0;
    }
    float distance = atof(istr);

    //printf("L_speed = %f | A_speed = %f | distance = %f \n", linear_speed, angular_speed, distance); // ������� 3 ���������
    Serial.print("L_speed = ");
    Serial.print(linear_speed);
    Serial.print(" | A_speed = ");
    Serial.print(angular_speed);
    Serial.print(" | distance = ");
    Serial.print(distance);
    Serial.println();
    miro.MoveDist(linear_speed, angular_speed, distance);
    return 0;
  }
  if (!strcmp(istr, "-d"))
  {
    //printf("miroset -d \n");
    Serial.println("miroset -d ");
    istr = strtok(NULL, " ");
    if (istr == NULL) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");  // ������ �������� ������ �������
      return -1;
    }

    int device = atoi(istr);
    if (device < 0 || device > 255) {
      /*printf("ERROR \n");*/ Serial.println("ERROR");  // ������ �������� ������ �������
      return -1;
    }

    const int count = 3; // ������ ���������� ����������
    float arg[count];
    for (int i = 0; i < count; i++)
    {
      istr = strtok(NULL, " ");
      if (istr == NULL) {
        /*printf("ERROR \n");*/ Serial.println("ERROR");  //break
        return -1;
      }

      arg[i] = atof(istr);
    }

    for (int i = 0; i < count; i++)
    {
      //printf("Argument%d = %f \n",i+1, arg[i]);
      Serial.print("Argument ");
      Serial.print(i + 1);
      Serial.print(" = ");
      Serial.print(arg[i]);
      Serial.println();
    }
    return 0;
  }
}

int CommLgcSerial::mirodevtable(char * str)
{
  Serial.println("MIRODEVTABLE CALL");
  for (int i = 0; i < miro.GetDeviceCount(); i++)
  {
    Serial.print(i);
    Serial.print("   -   ");
    Serial.print(miro.GetDeviceByIndex(i));
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
