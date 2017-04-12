#include "getcommands.h"
#include "midi.h"

GetCommands::GetCommands(Device *device)
    : SysExMessage(SysExMessage::GET_COMMAND_LIST, SysExMessage::QUERY,
                   device) {}
