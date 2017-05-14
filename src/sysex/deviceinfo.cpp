#include "deviceinfo.h"

DeviceInfo::DeviceInfo(Device *device)
    : SysExMessage(SysExMessage::GET_INFO, SysExMessage::QUERY, device) {
  retSetInfos = new std::map<DeviceInfoItem, RetSetInfo *>();
}

DeviceInfo::DeviceInfo(Device *device, ImplementedInfos *infoList)
    : SysExMessage(SysExMessage::GET_INFO, SysExMessage::QUERY, device),
      infoList(infoList) {
  retSetInfos = new std::map<DeviceInfoItem, RetSetInfo *>();
  std::vector<DeviceInfoItem> *implementedInfos =
      infoList->getImplementedInfos();
  for (std::vector<DeviceInfoItem>::iterator it = implementedInfos->begin();
       it < implementedInfos->end(); ++it) {
    this->setInfoItem(*it);
    execute();
    RetSetInfo *info = (RetSetInfo *)getAnswer();
    (*retSetInfos)[*it] = info;
  }
}

DeviceInfo::~DeviceInfo() { delete retSetInfos; }

BYTE_VECTOR *DeviceInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(this->infoItem);
  return messageData;
}

std::string DeviceInfo::getDataAsString() {
  if (data && data->size() > 0) {
    std::string result(data->begin() + 1, data->end());
    return result;
  } else {
    return std::string();
  }
}

std::string DeviceInfo::getItemValue(SysExMessage::DeviceInfoItem item) {
  RetSetInfo *i = (*retSetInfos)[item];
  return i->getValue();
}

std::map<SysExMessage::DeviceInfoItem, RetSetInfo *> *
DeviceInfo::getRetSetInfos() {
  return retSetInfos;
}

void DeviceInfo::deviceInfoChanged(SysExMessage::DeviceInfoItem item,
                                   std::string value) {
  std::cout << "DeviceInfo: Item " << item
            << " in DeviceInfoTable changed to value " << value << std::endl;
}

void DeviceInfo::createAnswer(SysExMessage::Command cmd,
                              std::vector<unsigned char> *message,
                              Device *device) {
  answer = new RetSetInfo(cmd, message, device);
  answer->parseAnswerData();
}
