#ifndef RETSETAUDIOCHANNELNAME_H
#define RETSETAUDIOCHANNELNAME_H

#include "portsysexmessage.h"

class RetSetAudioChannelName : public PortSysExMessage {
  public:
	RetSetAudioChannelName(Device *device);
	RetSetAudioChannelName(Command cmd, BYTE_VECTOR *message, Device *device)
		: PortSysExMessage(cmd, message, device) {}

	virtual ~RetSetAudioChannelName() override;
	void parseAnswerData() override;

	std::string getChannelName() const;
	void setChannelName(const std::string &getChannelName);

	unsigned int getNameLength() const;
	void setNameLength(unsigned int getNameLength);

	void setAudioChannelNumber(unsigned int iAudioChannelNumber);

	void setChannelDirection(const ChannelDirection &ChannelDirection);

  private:
	unsigned int m_iAudioChannelNumber = 0;
	ChannelDirection m_ChannelDirection = ChannelDirection::CD_NONE;
	unsigned int m_iMaximumChannelNameLength = 0;
	unsigned int m_iNameLength = 0;
	std::string m_sChannelName;
};

#endif // RETSETAUDIOCHANNELNAME_H
