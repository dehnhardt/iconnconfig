#include "devicedetectionquery.h"

DeviceDetectionQuery::DeviceDetectionQuery() : SysExMessage(new BYTE_VECTOR()) {

  transactionId = new BYTE_VECTOR();
  transactionId->push_back(0x00);
  transactionId->push_back(0x01);

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
}

DeviceDetectionQuery::~DeviceDetectionQuery() {
  delete transactionId;
  delete command;
  delete messageData;
}

BYTE_VECTOR *DeviceDetectionQuery::getMessageData() { return messageData; }
