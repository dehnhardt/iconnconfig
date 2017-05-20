#include "getmidiinfo.h"

GetMidiInfo::GetMidiInfo(Device *device)
		: SysExMessage(SysExMessage::GET_MIDI_INFO, SysExMessage::QUERY, device) {}
