#include "getinfolist.h"
#include "midi.h"

GetInfoList::GetInfoList(Device *device)
	: SysExMessage(Command::GET_INFO_LIST, SysExMessage::QUERY, device) {}
