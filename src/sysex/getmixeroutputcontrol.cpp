#include "getmixeroutputcontrol.h"

#include "retmixeroutputcontrol.h"

GetMixerOutputControl::GetMixerOutputControl(Device *device)
	: PortSysExMessage(GET_MIXER_OUTPUT_CONTROL, SysExMessage::QUERY, device) {}

GetMixerOutputControl::~GetMixerOutputControl() {}

void GetMixerOutputControl::createAnswer(Command m_Command,
										 std::vector<unsigned char> *message,
										 Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetMixerOutputControl>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetMixerOutputControl::getMessageData() {
	return getPortIdBytes();
}
