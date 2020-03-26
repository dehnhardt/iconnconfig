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
		if (apConfig.sinkChannelNumber > m_iMaxInputChannel)
			m_iMaxInputChannel = apConfig.sinkChannelNumber;
		if (apConfig.sourceChannelNumber > m_iMaxOutputChannel)
			m_iMaxOutputChannel = apConfig.sourceChannelNumber;
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

unsigned int RetSetAudioPatchbayParm::getMaxInputChannel() const {
	return m_iMaxInputChannel;
}

unsigned int RetSetAudioPatchbayParm::getMaxOutputChannel() const {
	return m_iMaxOutputChannel;
}

unsigned int RetSetAudioPatchbayParm::getNumberOfPatchbayConfigBlocks() const {
	return m_iNumberOfPatchbayConfigBlocks;
}

std::shared_ptr<
	std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
RetSetAudioPatchbayParm::getAudioPatchbayConfiguration() const {
	return m_pAudioPatchbayConfiguration;
}
