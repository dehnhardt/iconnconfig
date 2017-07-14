#include "getmidiportroute.h"
#include "retsetmidiportroute.h"

GetMidiPortRoute::GetMidiPortRoute(Device *device)
	: SysExMessage(Command::GET_MIDI_PORT_ROUTE, SysExMessage::QUERY, device) {}

BYTE_VECTOR *GetMidiPortRoute::getMessageData() {
	BYTE_VECTOR *messageData = MIDI::byteSplit(portNumber, 2);
	return messageData;
}

void GetMidiPortRoute::createAnswer(Command cmd, BYTE_VECTOR *message,
									Device *device) {
	answer = new RetSetMidiPortRoute(cmd, message, device);
	if (debug)
		answer->setDebug(true);
	answer->parseAnswerData();
}
