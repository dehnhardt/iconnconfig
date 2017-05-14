#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "implementedinfos.h"
#include "retsetinfo.h"
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
  DeviceInfo(Device *device);
  DeviceInfo(Device *device, ImplementedInfos *infoList);
  DeviceInfo(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}
  ~DeviceInfo();

  // methods
  void setInfoItem(DeviceInfoItem infoItem) { this->infoItem = infoItem; }
  BYTE_VECTOR *getMessageData();
  std::string getDataAsString();
  DeviceInfoItem getDeviceInfoItem();
  std::map<DeviceInfoItem, RetSetInfo *> *getRetSetInfos();
  void deviceInfoChanged(DeviceInfoItem item, std::string value);
  std::string getItemValue(SysExMessage::DeviceInfoItem item);

private:
  DeviceInfoItem infoItem = DEVICE_NAME;
  ImplementedInfos *infoList = 0;
  std::map<DeviceInfoItem, RetSetInfo *> *retSetInfos;

  // methods
private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device);
  void setInfoData(DeviceInfoItem item, std::string value);
};

#endif // DEVICEINFO_H
