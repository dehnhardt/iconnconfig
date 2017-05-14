#ifndef RETSETETHERNETPORTINFO_H
#define RETSETETHERNETPORTINFO_H

#include "sysexmessage.h"

class RetSetEthernetPortInfo : public SysExMessage {
public:
  RetSetEthernetPortInfo(Device *device);
  RetSetEthernetPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}
};

#endif // RETSETETHERNETPORTINFO_H
