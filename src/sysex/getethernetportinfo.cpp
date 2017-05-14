#include "getethernetportinfo.h"

GetEthernetPortInfo::GetEthernetPortInfo(Device *device)
    : SysExMessage(SysExMessage::GET_ETHERNET_PORT_INFO, SysExMessage::QUERY,
                   device) {}
BYTE_VECTOR *GetEthernetPortInfo::getMessageData() {
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(0x00);
  messageData->push_back(0x01);
  return messageData;
}
