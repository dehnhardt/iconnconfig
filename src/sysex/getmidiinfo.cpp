#include "getmidiinfo.h"

GetMidiInfo::GetMidiInfo(Device *device)
	: SysExMessage(Command::GET_MIDI_INFO, SysExMessage::QUERY, device) {}
