#include "retsetmidiportfilter.h"

RetSetMidiPortFilter::RetSetMidiPortFilter(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_FILTER, SysExMessage::QUERY,
				   device) {}
