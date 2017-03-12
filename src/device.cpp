#include "device.h"
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <array>

Device::Device( unsigned char deviceId[7]): deviceId(deviceId)
{
    int l = sizeof(this->deviceId);
    if( l >= 7 ){
        std::memcpy(productId, this->deviceId, 2);
        std::memcpy(serialNumber, this->deviceId +2, 5);
    }
    std::cout << "byte 1" << std::hex << productId[0] << " - byte 2" << std::hex << serialNumber[0];
}
