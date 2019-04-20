#ifndef GETMIXERPARM_H
#define GETMIXERPARM_H

#include "sysexmessage.h"

class GetMixerParm : public SysExMessage {
  public:
	GetMixerParm(Device *device);
	~GetMixerParm() override;
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice) override;

	unsigned int getAudioConfigurationNumber() const;
	void setAudioConfigurationNumber(unsigned int getAudioConfigurationNumber);

  protected:
	std::vector<unsigned char> *m_pGetMessageData() override;

  private:
	unsigned int m_iAudioConfigurationNumber = 0;
};

#endif // GETMIXERPARM_H
