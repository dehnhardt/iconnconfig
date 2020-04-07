#ifndef RETSETMIXEROUTPUTPARM_H
#define RETSETMIXEROUTPUTPARM_H

#include "portsysexmessage.h"

class RetSetMixerOutputParm : public PortSysExMessage {
  public:
	RetSetMixerOutputParm(Device *device);
	RetSetMixerOutputParm(Command cmd, BYTE_VECTOR *message, Device *device)
	    : PortSysExMessage(cmd, message, device) {}

	// SysExMessage interface
  public:
	void parseAnswerData() override;

	std::vector<unsigned int> getAudioSourceChannelNumbers() const {
		return m_vAudioSinkChannelIds;
	}

	std::string getMixerName() const;
	void setMixerName(const std::string &sMixerName);

	unsigned int getMixerSinkChannelNumber() const;
	void setMixerChannelNumber(unsigned int iMixerSinkNumber);

	void changeMixerOutputAssignment(AudioChannelId audioChanneId,
	                                 bool assigned);

  protected:
	std::vector<unsigned char> *getMessageData() override;

  private:
	unsigned char m_iNumberOfOutputAssignments = 0;
	unsigned char m_iMaxNameLength = 0;
	unsigned char m_iNameLength = 0;

	std::string m_sMixerName;
	AudioChannelId m_iChannelNumber = 0;
	std::vector<unsigned int> m_vAudioSinkChannelIds;
};

#endif // RETSETMIXEROUTPUTPARM_H
