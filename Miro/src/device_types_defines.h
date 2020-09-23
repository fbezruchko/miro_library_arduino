#ifndef device_types_h
#define device_types_h

#include "Led.h"
#include "Buzzer.h"
#include "Ldr.h"
#include "Line.h"
#include "Servo0.h"
#include "Hcsr04.h"

Device *createNULLDEVICE(uint8_t *pins)
{
  return nullptr;
}

void destroyNULLDEVICE(Device *device)
{
  return;
}

uint8_t getDefaultPinNULLDEVICE(uint8_t pinIndex)
{
  return 0;
}
char *getPinNameNULLDEVICE(uint8_t pinIndex)
{
  return 0;
}

#define NAME_VALUE(NAME, PINS, VALUE) NAME = VALUE
typedef enum
{
#include "device_types.h"
} DEVICE_TYPES_ENUM;
#undef NAME_VALUE

// #define NAME_VALUE(NAME, PINS, VALUE) NAME
// uint8_t DEVICE_TYPES_ID[] = {
// #include "device_types.h"
// };
// #undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) #NAME
const char *DEVICE_NAMES[] = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) PINS
const uint8_t DEVICE_PINS_COUNT[] = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &create##NAME
Device *(*DEVICE_CREATOR[NULLDEVICE + 1])(uint8_t *) = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &create##NAME
Device *(*DEVICE_CREATOR_DEFAULT[NULLDEVICE + 1])() = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &destroy##NAME
void (*DEVICE_DESTROYER[NULLDEVICE + 1])(Device *) = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &getDefaultPin##NAME
uint8_t (*DEVICE_GET_PIN[NULLDEVICE + 1])(uint8_t) = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &getPinName##NAME
const char *(*DEVICE_GET_PIN_NAME[NULLDEVICE + 1])(uint8_t) = {
#include "device_types.h"
};
#undef NAME_VALUE

#endif