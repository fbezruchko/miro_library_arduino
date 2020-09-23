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

  void help();
  int get();
  int set();
  void devTable();
  int calibWheel();
  void wheelTable();
  int attach();
  int detach();
  void devIDs();
};

#endif
