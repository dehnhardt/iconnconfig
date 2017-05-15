#ifndef RETSETETHERNETPORTINFO_H
#define RETSETETHERNETPORTINFO_H

#include "sysexmessage.h"

class RetSetEthernetPortInfo : public SysExMessage {
public:
  RetSetEthernetPortInfo(Device *device);

  RetSetEthernetPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {
    addresses = new std::map<int, std::string>();
  }

  enum IPFlags {
    STATIC = 0x00,      /*!< IP address is static */
    DYNAMIC = 0x01,     /*!< IP address is dynamic */
    ADDRESS = 0x02,     /*!< ADRRESS */
    SUBNET_MASK = 0x04, /*!< Subnet mask */
    GATEWAY = 0x08      /*!< Gateway */
  };

public:
  virtual void parseAnswerData();
  std::string getAddress(int flags);
  void setAddress(int flags, std::string address);
  IPFlags getMethod();

private:
  BYTE_VECTOR *portId = 0;
  IPFlags method;
  std::map<int, std::string> *addresses = 0;
};

#endif // RETSETETHERNETPORTINFO_H
