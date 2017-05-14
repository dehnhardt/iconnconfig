#ifndef ACK_H
#define ACK_H

#include "sysexmessage.h"

class Ack : public SysExMessage {
public:
  Ack(Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}
};

#endif // ACK_H
