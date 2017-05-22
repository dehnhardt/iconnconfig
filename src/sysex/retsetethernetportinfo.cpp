#include "retsetethernetportinfo.h"
#include "sstream"

RetSetEthernetPortInfo::RetSetEthernetPortInfo(Device *device)
    : SysExMessage(RET_SET_ETHERNET_PORT_INFO, QUERY, device) {
  addresses = new std::map<int, std::string>();
}

void RetSetEthernetPortInfo::parseAnswerData() {
  std::cout << "parseAnswerData" << std::endl;
  commandVersionNumber = data->at(0);
  portId = new BYTE_VECTOR(data->begin() + 1, data->begin() + 3);
  method = (IPFlags)data->at(3);

  setAddress(STATIC | ADDRESS, MIDI::decodeIp(data, 4));
  setAddress(STATIC | SUBNET_MASK, MIDI::decodeIp(data, 9));
  setAddress(STATIC | GATEWAY, MIDI::decodeIp(data, 14));
  setAddress(DYNAMIC | ADDRESS, MIDI::decodeIp(data, 19));
  setAddress(DYNAMIC | SUBNET_MASK, MIDI::decodeIp(data, 24));
  setAddress(DYNAMIC | GATEWAY, MIDI::decodeIp(data, 29));
  macAddress = std::string(data->begin() + 34, data->begin() + 46);
  int bonjourNameLength = data->at(46);
  bonjourName =
      std::string(data->begin() + 47, data->begin() + 47 + bonjourNameLength);
}

std::string RetSetEthernetPortInfo::getAddress(int flags) {
  return (*addresses)[flags];
}

void RetSetEthernetPortInfo::setAddress(int flags, std::string address) {
  (*addresses)[flags] = address;
}

std::string RetSetEthernetPortInfo::getMacAddress() const { return macAddress; }

std::string RetSetEthernetPortInfo::getBonjourName() const {
  return bonjourName;
}

void RetSetEthernetPortInfo::setBonjourName(const std::string &value) {
  bonjourName = value;
}

RetSetEthernetPortInfo::IPFlags RetSetEthernetPortInfo::getMethod() {
  return method;
}

void RetSetEthernetPortInfo::setMethod(RetSetEthernetPortInfo::IPFlags method) {
  this->method = method;
}

std::vector<unsigned char> *RetSetEthernetPortInfo::getMessageData() {
  this->command->at(0) = 0x40;
  BYTE_VECTOR *messageData = new BYTE_VECTOR();
  messageData->push_back(commandVersionNumber);
  messageData->insert(messageData->end(), portId->begin(), portId->end());
  messageData->push_back(method);
  BYTE_VECTOR *ip = MIDI::encodeIpAddress(getAddress(STATIC | ADDRESS));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  ip = MIDI::encodeIpAddress(getAddress(STATIC | SUBNET_MASK));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  ip = MIDI::encodeIpAddress(getAddress(STATIC | GATEWAY));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | ADDRESS));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | SUBNET_MASK));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  ip = MIDI::encodeIpAddress(getAddress(DYNAMIC | GATEWAY));
  messageData->insert(messageData->end(), ip->begin(), ip->end());
  messageData->insert(messageData->end(), macAddress.begin(), macAddress.end());
  messageData->push_back(bonjourName.size());
  messageData->insert(messageData->end(), bonjourName.begin(),
                      bonjourName.end());
  return messageData;
}
