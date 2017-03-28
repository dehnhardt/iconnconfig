#include "deviceinfo.h"

DeviceInfo::DeviceInfo(Device *device)
    : SysExMessage(SysExMessage::GET_INFO, SysExMessage::QUERY, device) {}

BYTE_VECTOR *DeviceInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(this->infoItem);
  return messageData;
}
