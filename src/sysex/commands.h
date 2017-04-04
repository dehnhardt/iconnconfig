#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "sysexmessage.h"

class Commands : public SysExMessage {
public:
  Commands(Device *device);
  Commands(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new Commands(cmd, message, device);
  }
};

#endif // GETCOMMANDS_H
