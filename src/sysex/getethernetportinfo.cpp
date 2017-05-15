#include "getethernetportinfo.h"
#include "retsetethernetportinfo.h"

GetEthernetPortInfo::GetEthernetPortInfo(Device *device)
    : SysExMessage(SysExMessage::GET_ETHERNET_PORT_INFO, SysExMessage::QUERY,
                   device) {}

BYTE_VECTOR *GetEthernetPortInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(0x00);
  messageData->push_back(0x01);
  return messageData;
}

void GetEthernetPortInfo::createAnswer(SysExMessage::Command cmd,
                                       BYTE_VECTOR *message, Device *device) {
  answer = new RetSetEthernetPortInfo(cmd, message, device);
  answer->parseAnswerData();
}
