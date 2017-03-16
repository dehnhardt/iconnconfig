#include "sysexmessage.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>

SysExMessage::SysExMessage(BYTE_VECTOR *deviceHeader) {
  this->deviceHeader = deviceHeader;
}

SysExMessage::~SysExMessage() {}

BYTE_VECTOR *SysExMessage::getMIDISysExMessage() {
  BYTE_VECTOR *body = new BYTE_VECTOR();
  BYTE_VECTOR *message = new BYTE_VECTOR();
  BYTE_VECTOR *manufacturerHeader = Device::getManufacturerHeader();

  /*
   * SYSEX_START 1Byte
   * MANUFACTURER_CODE 3Byte--| Header
   * MESSAGE_CLASS 1Byte ------
   *
   * PRODUCT_ID 2Byte --------| DeviceId -
   * SERIAL_NUMBER 5 Byte -----          |
   *                                     |
   * TRANSACTION_ID 2Byte ----|          | Body (Relevant for Checksum)
   * COMMAND 2Byte            |          |
   * DATA_LENGTH 2Byte        |          |
   * COMMAND_DATA xByte -------          |
   *                          -----------|
   * X_SUM 1yte
   *
   * SYSEX_END 1Byte
   */

  BYTE_VECTOR *bodyLength = MIDI::byteSplit(getMessageData()->size(), 2);

  body->reserve(deviceHeader->size() + 6 + getMessageData()->size() +
                getMessageData()->size());
  body->insert(body->end(), deviceHeader->begin(), deviceHeader->end());
  body->insert(body->end(), getTransactionId()->begin(),
               getTransactionId()->end());
  body->insert(body->end(), getCommand()->begin(), getCommand()->end());
  body->insert(body->end(), bodyLength->begin(), bodyLength->end());
  if (getMessageData()->size() > 0) {
    body->insert(body->end(), getMessageData()->begin(),
                 getMessageData()->end());
  }
  unsigned char cs = MIDI::RolandChecksum(body);

  message->reserve(manufacturerHeader->size() + deviceHeader->size() +
                   getMessageData()->size() + 4);
  message->push_back(SYSEX_START);
  message->insert(message->end(), manufacturerHeader->begin(),
                  manufacturerHeader->end());
  message->push_back(Device::MESSAGE_CLASS);
  message->insert(message->end(), body->begin(), body->end());
  message->push_back(cs);
  message->push_back(SYSEX_END);
  unsigned int nMessageSize = message->size();
  for (unsigned int i = 0; i < nMessageSize; i++)
    std::cout << std::hex << (int)message->at(i) << " ";
  std::cout << "\n";
  return message;
}
