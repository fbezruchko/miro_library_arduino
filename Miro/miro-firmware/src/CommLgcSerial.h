#ifndef commLgcserial_h
#define commLgcserial_h

#define RXBUFFERSIZE 64
#define TXBUFFERSIZE 32

int getDevIndex(char *devName);

class CommLgcSerial
{
public:
  void handle();
  void begin();

private:
  int parse(char *str);

  int help();
  int miroGet();
  int miroSet();
  int miroDevTable();
  int miroCalibWheel();
  int miroWheelTable();
  int miroAttach();
  int miroDetach();
  int miroGetDevName();
  int miroGetDevIndex();

  void printWheelTable();
};

#endif
