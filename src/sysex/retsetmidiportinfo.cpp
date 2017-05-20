#include "retsetmidiportinfo.h"

RetSetMidiPortInfo::RetSetMidiPortInfo(Device *device)
		: SysExMessage(SysExMessage::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
									 device) {}
