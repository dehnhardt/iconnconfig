#include "getmidiportremap.h"
#include "retsetmidiportremap.h"

GetMidiPortRemap::GetMidiPortRemap(Device *device)
    : SysExMessage(Command::GET_MIDI_PORT_REMAP, SysExMessage::QUERY, device) {
}

BYTE_VECTOR *GetMidiPortRemap::m_pGetMessageData() {
	BYTE_VECTOR *messageData =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortNumber), 2);
    messageData->push_back(static_cast<unsigned char>(m_iPortRemapDirection));
	return messageData;
}

void GetMidiPortRemap::createAnswer(Command cmd, BYTE_VECTOR *message,
									 Device *device) {
    m_pAnswer = new RetSetMidiPortRemap(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
