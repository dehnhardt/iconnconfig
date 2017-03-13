#ifndef DEVICE_H
#define DEVICE_H

#include <vector>


class Device
{
public:
    Device(long serialNumber, std::vector<unsigned char> * initString);

public:
    static const long MAUNFACTURER_USB_ID = 0x2321;
    static constexpr char MAUNFACTURER_SYSEX_ID [3] = {0x00, 0x01, 0x73};

private:
    long serialNumber;
    std::vector<unsigned char> * initString;
    int productId[2];
};

#endif // DEVICE_H
