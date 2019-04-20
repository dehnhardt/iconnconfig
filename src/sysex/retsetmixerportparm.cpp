#include "retsetmixerportparm.h"

RetSetMixerPortParm::RetSetMixerPortParm(Device *device)
	: SysExMessage(RET_SET_MIXER_PORT_PARM, SysExMessage::QUERY, device) {}

RetSetMixerPortParm::~RetSetMixerPortParm() {}

void RetSetMixerPortParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iNumberOfAudioPortMixerBlocks = m_pData->at(1);
	unsigned long offset = 2;
	for (unsigned int i = 0; i < m_iNumberOfAudioPortMixerBlocks; i++) {
		AudioPortMixerBlock apMixerBlock;
		apMixerBlock.audioPortId =
			static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, offset, 2));
		offset += 2;
		apMixerBlock.numberOfMixerInputs = m_pData->at(offset);
		offset++;
		apMixerBlock.numberOfMixerOutputs = m_pData->at(offset);
		offset++;
		m_vAudioPortMixerBlocks[apMixerBlock.audioPortId] = apMixerBlock;
	}
}

std::map<unsigned int, AudioPortMixerBlock>
RetSetMixerPortParm::getAudioPortMixerBlocks() const {
	return m_vAudioPortMixerBlocks;
}
