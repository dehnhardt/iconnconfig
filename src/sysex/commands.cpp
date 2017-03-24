#include "commands.h"
#include "midi.h"

Commands::Commands(Device *device)
    : SysExMessage(SysExMessage::GET_COMMAND_LIST, SysExMessage::QUERY,
                   device) {}
