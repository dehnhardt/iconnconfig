#include "retsetaudiopatchbayparm.h"
#include "../definitions.h"

RetSetAudioPatchbayParm::RetSetAudioPatchbayParm(Device *device)
	: PortSysExMessage(RET_SET_AUDIO_PATCHBAY_PARM, SysExMessage::QUERY,
					   device) {
	m_pAudioPatchbayConfiguration = std::make_shared<
		std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>();
}

RetSetAudioPatchbayParm::~RetSetAudioPatchbayParm() {}

void RetSetAudioPatchbayParm::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iNumberOfPatchbayConfigBlocks = m_pData->at(3);
	offset = 4;
	for (unsigned int i = 0; i < m_iNumberOfPatchbayConfigBlocks; i++) {
		AudioPatchbayConfiguration apConfig;
		apConfig.sinkChannelNumber = m_pData->at(offset);
		offset++;
		apConfig.sourceChannelNumber = m_pData->at(offset);
		offset++;
		apConfig.sourcePortId =
			static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, offset, 2));
		offset += 2;
		if (debug)
			std::cout << "Source Port: " << apConfig.sourcePortId
					  << ", SourceChannel: " << apConfig.sourceChannelNumber
					  << ", Sink Port: " << m_iPortId
					  << ", Sink Channel: " << apConfig.sinkChannelNumber
					  << std::endl;
		if ((apConfig.sourcePortId != 0) &&
			(apConfig.sourceChannelNumber != 0)) {
			AudioPortChannelId sinkKey =
				channelIndex(m_iPortId, AudioPortClass::PHYSICAL_PORT,
							 apConfig.sinkChannelNumber);
			AudioPortChannelId sourceKey = channelIndex(
				apConfig.sourcePortId, AudioPortClass::PHYSICAL_PORT,
				apConfig.sourceChannelNumber);
			std::map<AudioPortChannelId, bool> sources;
			try {
				sources = m_pAudioPatchbayConfiguration->at(sinkKey);
			} catch (__attribute__((unused)) const std::out_of_range &oor) {
				sources = std::map<AudioPortChannelId, bool>();
			}
			sources.insert(
				std::pair<AudioPortChannelId, bool>(sourceKey, true));
			m_pAudioPatchbayConfiguration->insert(
				std::pair<AudioPortChannelId,
						  std::map<AudioPortChannelId, bool>>(sinkKey,
															  sources));
		}
	}
}

unsigned int RetSetAudioPatchbayParm::getNumberOfPatchbayConfigBlocks() const {
	return m_iNumberOfPatchbayConfigBlocks;
}

std::shared_ptr<
	std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
RetSetAudioPatchbayParm::getAudioPatchbayConfiguration() const {
	return m_pAudioPatchbayConfiguration;
}

void RetSetAudioPatchbayParm::setAudioPatchbayConfiguration(
	std::shared_ptr<
		std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
		audioPatchbayConfiguration) {
	this->m_pAudioPatchbayConfiguration = audioPatchbayConfiguration;
}

std::vector<unsigned char> *RetSetAudioPatchbayParm::getMessageData() {
	BYTE_VECTOR *data = nullptr;
	BYTE_VECTOR *portId = nullptr;
	m_iCommandVersionNumber = 1;

	data = new BYTE_VECTOR();

	this->m_pCommandData->at(0) = 0x40;

	portId = MIDI::byteSplit7bit(m_iPortId, 2);
	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), portId->begin(), portId->end());

	data->push_back(
		static_cast<unsigned char>(m_pAudioPatchbayConfiguration->size()));

	for (auto configPairs : *m_pAudioPatchbayConfiguration) {
		AudioChannelId destChannel = configPairs.first % 100;
		data->push_back(static_cast<unsigned char>(destChannel));
		for (auto singleConfig : configPairs.second) {
			if (singleConfig.second) {
				AudioPortChannelId sourcePortChannelId = singleConfig.first;
				AudioPortClass sourcePortClass =
					AudioPortClass((sourcePortChannelId / 100) % 10);
				AudioPortId sourcePortId =
					(sourcePortChannelId / 100 - sourcePortClass) / 10;
				AudioChannelId sourceChannelId = sourcePortChannelId % 100;
				data->push_back(static_cast<unsigned char>(sourceChannelId));
				BYTE_VECTOR *vSourcePortId =
					MIDI::byteSplit7bit(sourcePortId, 2);
				data->insert(data->end(), vSourcePortId->begin(),
							 vSourcePortId->end());
				delete vSourcePortId;
			} else {
				data->push_back(0);
				data->push_back(0);
				data->push_back(0);
			}
		}
	}

	delete portId;
	return data;
}
