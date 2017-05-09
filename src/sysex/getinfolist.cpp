#include "infos.h"
#include "midi.h"

Infos::Infos(Device *device)
    : SysExMessage(SysExMessage::GET_INFO_LIST, SysExMessage::QUERY, device) {}
