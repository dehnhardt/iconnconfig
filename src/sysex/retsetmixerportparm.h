#ifndef RETSETMIXERPORTPARM_H
#define RETSETMIXERPORTPARM_H

#include "sysexmessage.h"

struct AudioPortMixerBlock {
	unsigned int audioPortId;
	unsigned int numberOfMixerInputs;
	unsigned int numberOfMixerOutputs;
};

class RetSetMixerPortParm : public SysExMessage {
  public:
	RetSetMixerPortParm(Device *device);
	RetSetMixerPortParm(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	~RetSetMixerPortParm() override;
	void parseAnswerData() override;

	std::map<unsigned int, AudioPortMixerBlock> getAudioPortMixerBlocks() const;

  private:
	unsigned int m_iNumberOfAudioPortMixerBlocks = 0;
	std::map<unsigned int, AudioPortMixerBlock> m_vAudioPortMixerBlocks;
};

#endif // RETSETMIXERPORTPARM_H
