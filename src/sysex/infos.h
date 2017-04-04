#ifndef INFOS_H
#define INFOS_H

#include "sysexmessage.h"

class Infos : public SysExMessage {
public:
  Infos(Device *device);
  Infos(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new Infos(cmd, message, device);
  }
};

#endif // INFOS_H
