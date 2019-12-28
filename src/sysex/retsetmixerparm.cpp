#include "retsetmixerparm.h"

RetSetMixerParm::RetSetMixerParm(Device *device)
	: SysExMessage(RET_SET_MIXER_PARM, SysExMessage::QUERY, device) {}

RetSetMixerParm::~RetSetMixerParm() {}

void RetSetMixerParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iActiveMixerConfigurationBlock = m_pData->at(1);
	m_iCurrentAudioConfigurationNumber = m_pData->at(2);
	m_iAudioConfigurationMixerBlocks = m_pData->at(3);
	unsigned long offset = 4;
	for (unsigned int i = 0; i < m_iAudioConfigurationMixerBlocks; i++) {
		AudioConfigurationMixerBlock acm;
		acm.mixerConfigurationNumber = m_pData->at(offset);
		offset++;
		acm.maximumNumberOfMixerInputs = m_pData->at(offset);
		offset++;
		acm.maximumNumberOfMixerOutputs = m_pData->at(offset);
		offset++;
		m_vACMixerBlocks.push_back(acm);
	}
}

unsigned int RetSetMixerParm::getActiveMixerConfigurationBlock() const {
	return m_iActiveMixerConfigurationBlock;
}

void RetSetMixerParm::setActiveMixerConfigurationBlock(
	unsigned int iActiveMixerConfigurationBlock) {
	m_iActiveMixerConfigurationBlock = iActiveMixerConfigurationBlock;
}

std::vector<AudioConfigurationMixerBlock>
RetSetMixerParm::getACMixerBlocks() const {
	return m_vACMixerBlocks;
}
