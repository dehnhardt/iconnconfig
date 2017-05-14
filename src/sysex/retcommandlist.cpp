#include "retcommandlist.h"
#include "midi.h"

RetCommandList::RetCommandList(Device *device)
    : SysExMessage(SysExMessage::RET_COMMAND_LIST, SysExMessage::QUERY,
                   device) {}

bool RetCommandList::isCommandSupported(const Command cmd) {
  if (!supportedCommands)
    return false;
  std::vector<Command>::iterator it;
  for (it = supportedCommands->begin(); it != supportedCommands->end(); it++) {
    if (*it == cmd)
      return true;
  }
  return false;
}

void RetCommandList::parseAnswerData() {
  supportedCommands = new std::vector<Command>();
  unsigned int nCommandsSize = data->size();
  Command c;
  for (unsigned int i = 0; i < nCommandsSize; i++) {
    c = (Command)data->at(i);
    if (c != 0)
      supportedCommands->push_back(c);
  }
}
