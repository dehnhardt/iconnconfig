#include "getmixerparm.h"
#include "retsetmixerparm.h"

GetMixerParm::GetMixerParm(Device *device)
	: SysExMessage(GET_MIXER_PARM, SysExMessage::QUERY, device) {}

GetMixerParm::~GetMixerParm() {}

void GetMixerParm::createAnswer(Command m_Command,
								std::vector<unsigned char> *message,
								Device *m_pDevice) {
	m_pAnswer =
		std::make_shared<RetSetMixerParm>(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetMixerParm::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	data->push_back(static_cast<unsigned char>(m_iAudioConfigurationNumber));
	return data;
}

unsigned int GetMixerParm::getAudioConfigurationNumber() const
{
	return m_iAudioConfigurationNumber;
}

void GetMixerParm::setAudioConfigurationNumber(unsigned int iAudioConfigurationNumber)
{
	m_iAudioConfigurationNumber = iAudioConfigurationNumber;
}
