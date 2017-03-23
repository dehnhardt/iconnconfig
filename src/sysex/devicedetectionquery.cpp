#include "devicedetectionquery.h"

DeviceDetectionQuery::DeviceDetectionQuery()
    : SysExMessage(SysExMessage::GET_DEVICE, SysExMessage::QUERY,
                   new BYTE_VECTOR()) {

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
}

DeviceDetectionQuery::~DeviceDetectionQuery() { delete transactionId; }
