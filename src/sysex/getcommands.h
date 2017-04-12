#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "commands.h"
#include "sysexmessage.h"

class GetCommands : public SysExMessage {
public:
  GetCommands(Device *device);
  GetCommands(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
    answer = new Commands(cmd, message, device);
    answer->parseAnswerData();
  }
};

#endif // GETCOMMANDS_H
