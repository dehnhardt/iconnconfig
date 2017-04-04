#include "deviceinfo.h"

DeviceInfo::DeviceInfo(Device *device)
    : SysExMessage(SysExMessage::GET_DEVICE_INFO, SysExMessage::QUERY, device) {
}

BYTE_VECTOR *DeviceInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(this->infoItem);
  return messageData;
}

std::string DeviceInfo::getDataAsString() {
  std::string result(data->begin() + 1, data->end());
  return result;
}

DeviceInfo::DeviceInfoItem DeviceInfo::getDeviceInfoItem() {
  return (DeviceInfoItem)(*data)[0];
}
