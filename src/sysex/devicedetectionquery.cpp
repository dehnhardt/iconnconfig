#include "devicedetectionquery.h"

DeviceDetectionQuery::DeviceDetectionQuery() : SysExMessage(new BYTE_VECTOR()) {

  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);
  deviceHeader->push_back(0x00);

  command = new BYTE_VECTOR();
  command->push_back(0x40);
  command->push_back(0x01);

  messageData = new BYTE_VECTOR();
  messageData->push_back(0x00);
  messageData->push_back(0x00);
}

BYTE_VECTOR *DeviceDetectionQuery::getMessageData() { return messageData; }

/*
BYTE_VECTOR *DeviceDetectionQuery::getMIDISysExMessage() {
  std::vector<unsigned char> message;
  message.clear();
  message.push_back(0xF0);

  message.push_back(0x00);
  message.push_back(0x01);
  message.push_back(0x73);
  message.push_back(0x7e);

  message.push_back(0x40);
  message.push_back(0x01);

  message.push_back(0x00);
  message.push_back(0x00);

  message.push_back(0x3E);
  message.push_back(0xF7);

  return 0;
}
*/
