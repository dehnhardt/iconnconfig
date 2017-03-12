#ifndef DEVICE_H
#define DEVICE_H


class Device
{
public:
    Device(unsigned char deviceId[7]);

private:
    unsigned char * deviceId;
    unsigned char productId[2];
    unsigned char serialNumber[5];
};

#endif // DEVICE_H
