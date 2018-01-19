#include "getmidiportinfo.h"

GetMidiPortInfo::GetMidiPortInfo(Device *device)
	: SysExMessage(GET_MIDI_PORT_INFO, SysExMessage::QUERY, device) {}

BYTE_VECTOR *GetMidiPortInfo::m_pGetMessageData() {
	BYTE_VECTOR *messageData =
		MIDI::byteSplit(static_cast<unsigned long>(m_iPortNumber), 2);
	return messageData;
}

void GetMidiPortInfo::createAnswer(Command cmd, BYTE_VECTOR *message,
								   Device *device) {
	m_pAnswer = new RetSetMidiPortInfo(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
