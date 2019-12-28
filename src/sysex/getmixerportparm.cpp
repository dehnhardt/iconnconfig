#include "getmixerportparm.h"
#include "retsetmixerportparm.h"

GetMixerPortParm::GetMixerPortParm(Device *device)
	: SysExMessage(GET_MIXER_PORT_PARM, SysExMessage::QUERY, device) {}

void GetMixerPortParm::createAnswer(Command m_Command,
									std::vector<unsigned char> *message,
									Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetSetMixerPortParm>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetMixerPortParm::getMessageData() {
	return new BYTE_VECTOR();
}
