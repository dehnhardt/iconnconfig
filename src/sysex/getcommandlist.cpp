#include "getcommandlist.h"
#include "midi.h"

GetCommandList::GetCommandList(Device *device)
    : SysExMessage(SysExMessage::GET_COMMAND_LIST, SysExMessage::QUERY,
                   device) {}
