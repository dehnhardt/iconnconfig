#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "implementedinfos.h"
#include "sysexmessage.h"

#include <QtCore/QCoreApplication>

class InfoItem {

public:
	SysExMessage::DeviceInfoItem infoItem;
	std::string name;
	std::string value;
	bool editable;
};

class DeviceInfo : public SysExMessage {

	Q_DECLARE_TR_FUNCTIONS(InfoItem)

public:
  // methods
public:
  DeviceInfo(Device *device);
	DeviceInfo(Device *device, ImplementedInfos *infoList);
	DeviceInfo(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}
	~DeviceInfo();

  void setInfoItem(DeviceInfoItem infoItem) { this->infoItem = infoItem; }
  BYTE_VECTOR *getMessageData();
  std::string getDataAsString();
  DeviceInfoItem getDeviceInfoItem();
	bool isItemEditable(DeviceInfoItem item);
	std::string getItemName(DeviceInfoItem item);
	std::string getItemValue(DeviceInfoItem item);
	std::vector<InfoItem> *getDeviceInfos();

	// members
private:
  DeviceInfoItem infoItem = DEVICE_NAME;
	ImplementedInfos *infoList = 0;
	std::vector<InfoItem> *deviceInfos;
	std::map<DeviceInfoItem, InfoItem> *mappedInfos;

	// methods
private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new DeviceInfo(cmd, message, device);
  }
};

#endif // DEVICEINFO_H
