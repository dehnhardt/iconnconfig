#ifndef RETSETMIXERPARM_H
#define RETSETMIXERPARM_H

#include "sysexmessage.h"

struct AudioConfigurationMixerBlock {
	unsigned int mixerConfigurationNumber;
	unsigned int maximumNumberOfMixerInputs;
	unsigned int maximumNumberOfMixerOutputs;
};

class RetSetMixerParm : public SysExMessage {
  public:
	RetSetMixerParm(Device *device);
	RetSetMixerParm(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	virtual ~RetSetMixerParm() override;
	void parseAnswerData() override;

	unsigned int getActiveMixerConfigurationBlock() const;
	void setActiveMixerConfigurationBlock(
		unsigned int getActiveMixerConfigurationBlock);

	std::vector<AudioConfigurationMixerBlock> getACMixerBlocks() const;

  private:
	unsigned int m_iActiveMixerConfigurationBlock = 0;
	unsigned int m_iCurrentAudioConfigurationNumber = 0;
	unsigned int m_iAudioConfigurationMixerBlocks = 0;
	std::vector<AudioConfigurationMixerBlock> m_vACMixerBlocks;
};

#endif // RETSETMIXERPARM_H
