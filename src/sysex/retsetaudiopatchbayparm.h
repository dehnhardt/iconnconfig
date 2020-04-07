#ifndef RETSETAUDIOPATCHBAYPARM_H
#define RETSETAUDIOPATCHBAYPARM_H

#include "portsysexmessage.h"

typedef struct {
	unsigned int sinkChannelNumber;
	unsigned int sourcePortId;
	unsigned int sourceChannelNumber;
} AudioPatchbayConfiguration;

class RetSetAudioPatchbayParm : public PortSysExMessage {
  public:
	RetSetAudioPatchbayParm(Device *device);

	RetSetAudioPatchbayParm(Command cmd, BYTE_VECTOR *message, Device *device)
	    : PortSysExMessage(cmd, message, device) {
		m_pAudioPatchbayConfiguration = std::make_shared<
		    std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>();
	}

	virtual ~RetSetAudioPatchbayParm() override;
	void parseAnswerData() override;

	unsigned int getNumberOfPatchbayConfigBlocks() const;

	std::shared_ptr<
	    std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
	getAudioPatchbayConfiguration() const;

	void setAudioPatchbayConfiguration(
	    std::shared_ptr<
	        std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>);

  private:
	std::shared_ptr<
	    std::map<AudioPortChannelId, std::map<AudioPortChannelId, bool>>>
	    m_pAudioPatchbayConfiguration = nullptr;
	unsigned int m_iNumberOfPatchbayConfigBlocks = 0;

	// SysExMessage interface
  protected:
	virtual std::vector<unsigned char> *getMessageData() override;
};

#endif // RETSETAUDIOPATCHBAYPARM_H
