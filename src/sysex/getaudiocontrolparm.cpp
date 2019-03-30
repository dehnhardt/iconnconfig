#include "getaudiocontrolparm.h"
#include "retsetaudiocontrolparm.h"

GetAudioControlParm::GetAudioControlParm(Device *device)
	: PortSysExMessage(GET_AUDIO_CONTROL_PARM, SysExMessage::QUERY, device) {}

GetAudioControlParm::~GetAudioControlParm() {}

void GetAudioControlParm::createAnswer(Command m_Command,
									   std::vector<unsigned char> *message,
									   Device *m_pDevice) {
	m_pAnswer = std::make_shared<RetSetAudioControlParm>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioControlParm::m_pGetMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *vPortId =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	data->insert(data->begin(), vPortId->begin(), vPortId->end());
	data->push_back(static_cast<unsigned char>(m_iControllerNumber));
	return data;
}

unsigned int GetAudioControlParm::getControllerNumber() const {
	return m_iControllerNumber;
}

void GetAudioControlParm::setControllerNumber(unsigned int iControllerNumber) {
	m_iControllerNumber = iControllerNumber;
}
