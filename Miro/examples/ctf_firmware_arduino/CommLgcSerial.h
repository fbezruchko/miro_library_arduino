#ifndef commLgcserial_h
#define commLgcserial_h

#define RXBUFFERSIZE 64

class CommLgcSerial {
  public:
  void handle();
  void begin();

  private:
    int parse(char *str);
    
    int miromove(char * str);
};

#endif
