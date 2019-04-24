#ifndef commLgcserial_h
#define commLgcserial_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

#define RXBUFFERSIZE 128

#include <string.h> 
//#include <stdio.h>
#include <stdlib.h>

class CommLgcSerial {
public:
    void handle();
    void begin();

private:
    int parse(char *str);

    int miroget(char *str);
    int miroset(char *str);
    int mirodevtable(char *str);
};


#endif
