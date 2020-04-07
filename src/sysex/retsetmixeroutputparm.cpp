#include "retsetmixeroutputparm.h"

RetSetMixerOutputParm::RetSetMixerOutputParm(Device *device)
	: PortSysExMessage(RET_SET_MIXER_OUTPUT_PARM, SysExMessage::QUERY, device) {
}

void RetSetMixerOutputParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin8bit(m_pData, 1, 2));
	m_iChannelNumber = m_pData->at(3);
	m_iNumberOfOutputAssignments = m_pData->at(4);
	unsigned long offset = 5;
	for (unsigned int i = 0; i < m_iNumberOfOutputAssignments; i++) {
		m_vAudioSinkChannelIds.push_back(m_pData->at(offset));
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

	data = new BYTE_VECTOR();

	this->m_pCommandData->at(0) = 0x40;

	portId = MIDI::byteSplit7bit(m_iPortId, 2);
	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), portId->begin(), portId->end());

	m_iNumberOfOutputAssignments =
		static_cast<unsigned char>(m_vAudioSinkChannelIds.size());

	data->push_back(static_cast<unsigned char>(m_iChannelNumber));
	data->push_back(m_iNumberOfOutputAssignments);
	for (unsigned int channel : m_vAudioSinkChannelIds) {
		data->push_back(static_cast<unsigned char>(channel));
	}
	data->push_back(m_iMaxNameLength);
	data->push_back(m_iNameLength);
	data->insert(data->end(), m_sMixerName.begin(), m_sMixerName.end());

	delete portId;
	return data;
}

unsigned int RetSetMixerOutputParm::getMixerSinkChannelNumber() const {
	return m_iChannelNumber;
}

void RetSetMixerOutputParm::setMixerChannelNumber(
	unsigned int iMixerSourcePortNumber) {
	m_iChannelNumber = iMixerSourcePortNumber;
}

void RetSetMixerOutputParm::changeMixerOutputAssignment(
	AudioChannelId audioChanneId, bool assigned) {
	if (audioChanneId == 0) {
		m_vAudioSinkChannelIds.clear();
	} else {
		if (assigned) {
			for (AudioChannelId existingAudioChannelId :
				 m_vAudioSinkChannelIds) {
				if (existingAudioChannelId == audioChanneId)
					return;
			}
			m_vAudioSinkChannelIds.push_back(audioChanneId);
		} else {
			for (std::vector<unsigned int>::iterator it =
					 m_vAudioSinkChannelIds.begin();
				 it < m_vAudioSinkChannelIds.end(); ++it) {
				if (*it == audioChanneId)
					m_vAudioSinkChannelIds.erase(it);
			}
		}
	}
	m_iNumberOfOutputAssignments =
		static_cast<unsigned char>(m_vAudioSinkChannelIds.size());
}

std::string RetSetMixerOutputParm::getMixerName() const { return m_sMixerName; }

void RetSetMixerOutputParm::setMixerName(const std::string &sMixerName) {
	m_sMixerName = sMixerName;
}
