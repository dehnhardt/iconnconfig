#ifndef COMMANDS_H
#define COMMANDS_H

#include "sysexmessage.h"

#include <vector>

class RetCommandList : public SysExMessage {
public:
  RetCommandList(Device *device);
  RetCommandList(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}
  std::vector<int> getSupportedCommands();
  bool isCommandSupported(Command cmd);
  void parseAnswerData();

private:
  std::vector<Command> *supportedCommands = 0;
};

#endif // GETCOMMANDS_H
