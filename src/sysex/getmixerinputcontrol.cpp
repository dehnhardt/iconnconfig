#include "getmixerinputcontrol.h"
#include "retmixerinputcontrol.h"

GetMixerInputControl::GetMixerInputControl(Device *device)
	: PortSysExMessage(GET_MIXER_INPUT_CONTROL, SysExMessage::QUERY, device) {}

GetMixerInputControl::~GetMixerInputControl() {}

void GetMixerInputControl::createAnswer(Command m_Command,
										std::vector<unsigned char> *message,
										Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetMixerInputControl>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetMixerInputControl::getMessageData() {
	return getPortIdBytes();
}
