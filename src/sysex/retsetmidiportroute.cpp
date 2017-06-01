#include "retsetmidiportroute.h"

RetSetMidiPortRoute::RetSetMidiPortRoute(Device *device)
		: SysExMessage(SysExMessage::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
									 device) {}

void RetSetMidiPortRoute::parseAnswerData() {
	portId = MIDI::byteJoin(data, 1, 2);
	portRoutings = new BYTE_VECTOR(data->begin() + 2, data->end() - 2);
}
