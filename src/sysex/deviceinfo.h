#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "sysexmessage.h"

class DeviceInfo : public SysExMessage {
public:
  // methods
public:
  DeviceInfo(Device *device);
  DeviceInfo(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

  void setInfoItem(DeviceInfoItem infoItem) { this->infoItem = infoItem; }
  BYTE_VECTOR *getMessageData();
  std::string getDataAsString();
  DeviceInfoItem getDeviceInfoItem();

private:
  DeviceInfoItem infoItem = DEVICE_NAME;

  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new DeviceInfo(cmd, message, device);
  }
};

#endif // DEVICEINFO_H
