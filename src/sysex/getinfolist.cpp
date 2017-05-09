#include "getinfolist.h"
#include "midi.h"

GetInfoList::GetInfoList(Device *device)
		: SysExMessage(SysExMessage::GET_INFO_LIST, SysExMessage::QUERY, device) {}
