#ifndef GETAUDIOCHANNELNAME_H
#define GETAUDIOCHANNELNAME_H

#include "portsysexmessage.h"

class GetAudioChannelName : public PortSysExMessage {
  public:
	GetAudioChannelName(Device *device);

	virtual ~GetAudioChannelName();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

	void setChannelNumber(unsigned int iChannelNumber);
	void setChannelDirection(const pk::ChannelDirection &channelDirection);

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	unsigned int m_iChannelNumber = 0;
	pk::ChannelDirection m_channelDirection = pk::ChannelDirection::CD_NONE;
};

#endif // GETAUDIOCHANNELNAME_H
