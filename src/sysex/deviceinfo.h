#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "implementedinfos.h"
#include "sysexmessage.h"

class DeviceInfo : public SysExMessage {
public:
  // methods
public:
  DeviceInfo(Device *device);
	DeviceInfo(Device *device, ImplementedInfos *infoList);
	DeviceInfo(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

  void setInfoItem(DeviceInfoItem infoItem) { this->infoItem = infoItem; }
  BYTE_VECTOR *getMessageData();
  std::string getDataAsString();
  DeviceInfoItem getDeviceInfoItem();

	// members
private:
  DeviceInfoItem infoItem = DEVICE_NAME;
	ImplementedInfos *infoList = 0;

	// methods
private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new DeviceInfo(cmd, message, device);
  }
};

class InfoItem {
public:
	std::string name;
	std::string value;
	bool editable;
};

#endif // DEVICEINFO_H
