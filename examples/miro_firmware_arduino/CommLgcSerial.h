#ifndef commLgcserial_h
#define commLgcserial_h

#define RXBUFFERSIZE 64

#define CMDBUFFERSIZE 2
#define CMDLENGTH 32

class CommLgcSerial {
  public:
    void handle();
    void begin();

  private:
    int parse(char *str);

    int prevCmd();
    int nextCmd();
    
    int help();

    int miroget(char *str);
    int miroset(char *str);

    int mirodevtable(char *str);
    int mirocalibwheel(char *str);
    int mirowheeltable(char * str);
    int miromode(char * str);

    void printWheelTable();
};

#endif
