#include "Device.h"

void Device::Sync()
{
	return;
}

byte Device::GetParamCount()
{
    return this->_param_count;
}

char* Device::GetName()
{
    return this->_dev_name;
}
