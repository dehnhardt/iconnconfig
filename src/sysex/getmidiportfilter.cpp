#include "getmidiportfilter.h"

GetMidiPortFilter::GetMidiPortFilter(Device *device)
	: SysExMessage(Command::GET_MIDI_PORT_FILTER, SysExMessage::QUERY, device) {
}
