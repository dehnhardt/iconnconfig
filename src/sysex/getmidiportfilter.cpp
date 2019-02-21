#include "getmidiportfilter.h"
#include "retsetmidiportfilter.h"

GetMidiPortFilter::GetMidiPortFilter(Device *device)
	: SysExMessage(Command::GET_MIDI_PORT_FILTER, SysExMessage::QUERY, device) {
}

BYTE_VECTOR *GetMidiPortFilter::m_pGetMessageData() {
	BYTE_VECTOR *messageData =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortNumber), 2);
	messageData->push_back(static_cast<unsigned char>(m_iPortFilterDirection));
	return messageData;
}

void GetMidiPortFilter::createAnswer(Command cmd, BYTE_VECTOR *message,
									 Device *device) {
	m_pAnswer = new RetSetMidiPortFilter(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
