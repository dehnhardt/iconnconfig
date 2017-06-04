#include "retsetmidiportroute.h"

#include <cmath>

RetSetMidiPortRoute::RetSetMidiPortRoute(Device *device)
		: SysExMessage(SysExMessage::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
									 device) {}

void RetSetMidiPortRoute::parseAnswerData() {
	portId = MIDI::byteJoin(data, 1, 2);
	portRoutings = new BYTE_VECTOR(data->begin() + 3, data->end());
}

bool RetSetMidiPortRoute::isPortRouted(int portNumber) {
	--portNumber;
	unsigned int byte = portNumber / 4;
	unsigned int bit = portNumber - (byte * 4);
	if( byte < portRoutings->size() ){
		unsigned int byteVal = portRoutings->at(byte);
		int mask = pow(2, bit);
		return byteVal & mask;
	}
	return false;
}
