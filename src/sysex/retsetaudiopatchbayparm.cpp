#include "retsetaudiopatchbayparm.h"

RetSetAudioPatchbayParm::RetSetAudioPatchbayParm(Device *device)
	: PortSysExMessage(RET_SET_AUDIO_PATCHBAY_PARM, SysExMessage::QUERY,
					   device) {
	m_pAudioPatchbayConfiguration =
		std::make_shared<std::vector<AudioPatchbayConfiguration>>();
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
		apConfig.inputChannelNumber = m_pData->at(offset);
		offset++;
		apConfig.outputChannelNumber = m_pData->at(offset);
		offset++;
		apConfig.outputPortId =
			static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, offset, 2));
		offset += 2;
		if (apConfig.inputChannelNumber > m_iMaxInputChannel)
			m_iMaxInputChannel = apConfig.inputChannelNumber;
		if (apConfig.outputChannelNumber > m_iMaxOutputChannel)
			m_iMaxOutputChannel = apConfig.outputChannelNumber;
		if (debug)
			std::cout << "Input Port: " << m_iPortId
					  << ", Input Channel: " << apConfig.inputChannelNumber
					  << ", Output Port: " << apConfig.outputPortId
					  << ", OutputChannel: " << apConfig.outputChannelNumber
					  << std::endl;
		m_pAudioPatchbayConfiguration->push_back(apConfig);
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

std::shared_ptr<std::vector<AudioPatchbayConfiguration>>
RetSetAudioPatchbayParm::getAudioPatchbayConfiguration() const {
	return m_pAudioPatchbayConfiguration;
}
