#ifndef commLgcserial_h
#define commLgcserial_h

#define RXBUFFERSIZE 64
#define TXBUFFERSIZE 32

class CommLgcSerial {
  public:
    void handle();
    void begin();

  private:
    int parse(char *str);
    
    int help();
    int miroget();
    int miroset();
    int mirodevtable();
    int mirocalibwheel();
    int mirowheeltable();
    int miroattach();
    int mirodetach();

    void printWheelTable();
};

#endif
