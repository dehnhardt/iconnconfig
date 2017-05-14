#include "getdevice.h"

GetDevice::GetDevice()
    : SysExMessage(SysExMessage::GET_DEVICE, SysExMessage::QUERY, 0) {

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

GetDevice::~GetDevice() { delete transactionId; }
