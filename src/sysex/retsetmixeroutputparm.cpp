#include "retsetmixeroutputparm.h"

RetSetMixerOutputParm::RetSetMixerOutputParm(Device *device)
	: PortSysExMessage(RET_SET_MIXER_OUTPUT_PARM, SysExMessage::QUERY, device) {
}

void RetSetMixerOutputParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin8bit(m_pData, 1, 2));
	m_iMixerSinkChannelNumber = m_pData->at(3);
	m_iNumberOfOutputAssignments = m_pData->at(4);
	unsigned long offset = 5;
	for (unsigned int i = 0; i < m_iNumberOfOutputAssignments; i++) {
		m_vAudioSourceChannelIds.push_back(
			m_pData->at(offset + static_cast<unsigned long>(i)));
		++offset;
	}
	m_iMaxNameLength = m_pData->at(offset);
	++offset;
	m_iNameLength = m_pData->at(offset);
	++offset;
	m_sMixerName = std::string(m_pData->begin() + static_cast<long>(offset),
							   m_pData->begin() + static_cast<long>(offset) +
								   m_iNameLength);
}

std::vector<unsigned char> *RetSetMixerOutputParm::getMessageData() {
	BYTE_VECTOR *data = nullptr;
	BYTE_VECTOR *portId = nullptr;

	this->m_pCommandData->at(0) = 0x40;

	portId = MIDI::byteSplit7bit(m_iPortId);
	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), portId->begin(), portId->end());

	data->push_back(m_iNumberOfOutputAssignments);
	for (unsigned int channel : m_vAudioSourceChannelIds) {
		data->push_back(static_cast<unsigned char>(channel));
	}
	data->push_back(m_iMaxNameLength);
	data->push_back(m_iNameLength);
	data->insert(data->end(), m_sMixerName.begin(), m_sMixerName.end());
	return data;
}

unsigned int RetSetMixerOutputParm::getMixerSinkChannelNumber() const {
	return m_iMixerSinkChannelNumber;
}

void RetSetMixerOutputParm::setMixerSinkChannelNumber(
	unsigned int iMixerSourcePortNumber) {
	m_iMixerSinkChannelNumber = iMixerSourcePortNumber;
}

std::string RetSetMixerOutputParm::getMixerName() const { return m_sMixerName; }

void RetSetMixerOutputParm::setMixerName(const std::string &sMixerName) {
	m_sMixerName = sMixerName;
}
