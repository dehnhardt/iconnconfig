#ifndef GETETHERNETPORTINFO_H
#define GETETHERNETPORTINFO_H

#include "sysexmessage.h"

class GetEthernetPortInfo : public SysExMessage {
public:
  GetEthernetPortInfo(Device *device);
  GetEthernetPortInfo(SysExMessage::Command cmd, BYTE_VECTOR *message,
                      Device *device)
      : SysExMessage(cmd, message, device) {}

protected:
  std::vector<unsigned char> *getMessageData();
};

#endif // GETETHERNETPORTINFO_H
