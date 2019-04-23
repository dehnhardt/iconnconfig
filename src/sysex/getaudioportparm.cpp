#include "getaudioportparm.h"
#include "retsetaudioportparm.h"

GetAudioPortParm::GetAudioPortParm(Device *device)
	: SysExMessage(GET_AUDIO_PORT_PARM, QUERY, device) {}

GetAudioPortParm::~GetAudioPortParm() {}

void GetAudioPortParm::createAnswer(Command cmd, BYTE_VECTOR *message,
									Device *device) {
	m_pAnswer = std::make_shared<RetSetAudioPortParm>(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioPortParm::getMessageData() {
	BYTE_VECTOR *messageData =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	return messageData;
}
