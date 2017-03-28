#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "sysexmessage.h"

class DeviceInfo : public SysExMessage {
public:
  enum DeviceInfoItem {
    ACESSORY_NAME = 0x01, /*!< Name of acessory */
    MANUFACTURER_NAME,    /*!< Name of device manufacturer manufacturer */
    MODEL_NUMBER,         /*!< Devices modelnumber */
    SERIAL_NUMBER,        /*!< Devices serial number */
    FIRMWARE_VERSION,     /*!< Installed firmware version */
    HARDWARE_VERSION,     /*!< Hardware version of device */
    DEVICE_NAME = 0x10    /*!< Name of device (writable) */
  };
  DeviceInfo(Device *device);
  void setInfoItem(DeviceInfoItem infoItem) { this->infoItem = infoItem; }
  BYTE_VECTOR *getMessageData();

private:
  DeviceInfoItem infoItem = DEVICE_NAME;
};

#endif // DEVICEINFO_H
