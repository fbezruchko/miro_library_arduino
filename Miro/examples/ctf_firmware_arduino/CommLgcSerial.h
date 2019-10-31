#ifndef commLgcserial_h
#define commLgcserial_h

#define RXBUFFERSIZE 64

//const char msgs_error[][8]={"MIRO: Unknown command!", "ERROR", ">>>NO ARGUMENTS", ">>>UNKNOWN DEVICE INDEX", ">>>NO PARAMETERS", ">>>UNKNOWN PARAMETER", ">>>UNKNOWN PARAMETER", ">>>UNKNOWN PARAMETER VALUE"};

class CommLgcSerial {
  public:
  void handle();
  void begin();

  private:
    int parse(char *str);

    int miroget(char *str);
    int miroset(char *str);
    
    int mirodevtable(char *str);
    int miromode(char * str);
};

#endif
