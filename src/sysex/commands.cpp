#include "commands.h"

Commands::Commands(BYTE_VECTOR *deviceHeader)
    : SysExMessage(SysExMessage::GET_COMMAND_LIST, SysExMessage::QUERY,
                   deviceHeader) {}
