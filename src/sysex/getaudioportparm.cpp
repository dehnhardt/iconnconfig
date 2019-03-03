#include "getaudioportparm.h"
#include "retsetaudioportparm.h"

GetAudioPortParm::GetAudioPortParm(Device *device)
	: SysExMessage(GET_AUDIO_PORT_PARM, QUERY, device) {}

GetAudioPortParm::~GetAudioPortParm() {}

void GetAudioPortParm::createAnswer(Command cmd, BYTE_VECTOR *message,
									Device *device) {
	m_pAnswer = new RetSetAudioPortParm(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioPortParm::m_pGetMessageData() {
	BYTE_VECTOR *messageData =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	return messageData;
}
