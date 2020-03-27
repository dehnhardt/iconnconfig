#include "retsetmixerinputparm.h"

RetSetMixerInputParm::RetSetMixerInputParm(Device *device)
	: PortSysExMessage(RET_SET_MIXER_INPUT_PARM, SysExMessage::QUERY, device) {}

void RetSetMixerInputParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin8bit(m_pData, 1, 2));
	m_iMixerInputNumber = m_pData->at(3);
	m_iAudioSourcePortId =
		static_cast<unsigned int>(MIDI::byteJoin8bit(m_pData, 4, 2));
	m_iAudioSourceChannelNumber = m_pData->at(6);
}

std::vector<unsigned char> *RetSetMixerInputParm::getMessageData() {
	BYTE_VECTOR *data = nullptr;
	BYTE_VECTOR *portId = nullptr;
	BYTE_VECTOR *audioSourcePortId = new BYTE_VECTOR();

	data = new BYTE_VECTOR();

	this->m_pCommandData->at(0) = 0x40;

	portId = MIDI::byteSplit7bit(m_iPortId, 2);
	audioSourcePortId = MIDI::byteSplit7bit(m_iAudioSourcePortId, 2);

	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_iMixerInputNumber));
	data->insert(data->end(), audioSourcePortId->begin(),
				 audioSourcePortId->end());
	data->push_back(static_cast<unsigned char>(m_iAudioSourceChannelNumber));
	delete portId;
	delete audioSourcePortId;
	return data;
}

unsigned int RetSetMixerInputParm::getAudioSourceChannelNumber() const {
	return m_iAudioSourceChannelNumber;
}

void RetSetMixerInputParm::setAudioSourceChannelNumber(
	unsigned int iAudioSourceChannelNumber) {
	m_iAudioSourceChannelNumber = iAudioSourceChannelNumber;
}

unsigned int RetSetMixerInputParm::getAudioSourcePortId() const {
	return m_iAudioSourcePortId;
}

void RetSetMixerInputParm::setAudioSourcePortId(
	unsigned int iAudioSourcePortId) {
	m_iAudioSourcePortId = iAudioSourcePortId;
}
