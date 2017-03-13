#include "device.h"
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <array>

Device::Device( long serialNumber, std::vector<unsigned char> * initString ):serialNumber(serialNumber), initString(initString)
{

}
