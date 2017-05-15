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

  MIDI::encodeIpAddress(getAddress(DYNAMIC | ADDRESS));
}

std::string RetSetEthernetPortInfo::getAddress(int flags) {
  return (*addresses)[flags];
}

void RetSetEthernetPortInfo::setAddress(int flags, std::string address) {
  (*addresses)[flags] = address;
}

RetSetEthernetPortInfo::IPFlags RetSetEthernetPortInfo::getMethod() {
  return method;
}
