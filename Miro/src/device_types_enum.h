#ifndef device_types_h
#define device_types_h

#include "Led.h"

Device* CreateNULLDEVICE(uint8_t* pin)
{
  return nullptr;
}

void DestroyNULLDEVICE(Device* device)
{
  return;
}

//namespace miro {

#define NAME_VALUE(NAME, PINS, VALUE) NAME = VALUE
typedef enum {
#include "device_types.h"
} DEVICE_TYPES_ENUM;
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) NAME
unsigned int DEVICE_TYPES_ID[] = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) #NAME
const char* DEVICE_TYPES_NAMES[] = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) PINS
const uint8_t DEVICE_PINS_COUNT[] = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &Create##NAME
Device* (*DEVICE_FABRIC[NULLDEVICE+1])(uint8_t *) = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &Create##NAME
Device* (*DEVICE_FABRIC_DEFAULT[NULLDEVICE+1])() = {
#include "device_types.h"
};
#undef NAME_VALUE

#define NAME_VALUE(NAME, PINS, VALUE) &Destroy##NAME
void (*DEVICE_DESTROYER[NULLDEVICE+1])(Device *) = {
#include "device_types.h"
};
#undef NAME_VALUE

uint8_t getDeviceIdByName(char* name)
{
	uint8_t id;
	
	for (id = 0; id < (NULLDEVICE + 1); id++)
	{
		if (!strcmp(name, DEVICE_TYPES_NAMES[id])) break;
	}
	return id;
}

//} // end namespace

#endif