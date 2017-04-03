#include "deviceinfo.h"

DeviceInfo::DeviceInfo(Device *device)
    : SysExMessage(SysExMessage::GET_DEVICE_INFO, SysExMessage::QUERY, device) {
}

BYTE_VECTOR *DeviceInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(this->infoItem);
  return messageData;
}

void DeviceInfo::createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                              Device *device) {
  answer = new DeviceInfo(cmd, message, device);
}
