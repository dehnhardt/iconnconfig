#include "getmidiportinfo.h"

GetMidiPortInfo::GetMidiPortInfo(Device *device)
		: SysExMessage(SysExMessage::GET_MIDI_PORT_INFO, SysExMessage::QUERY,
									 device) {}

BYTE_VECTOR *GetMidiPortInfo::getMessageData() {
	BYTE_VECTOR *messageData = MIDI::byteSplit(portNumber, 2);
	return messageData;
}

void GetMidiPortInfo::createAnswer(SysExMessage::Command cmd,
																	 BYTE_VECTOR *message, Device *device) {
	answer = new RetSetMidiPortInfo(cmd, message, device);
	if (debug)
		answer->setDebug(true);
	answer->parseAnswerData();
}
