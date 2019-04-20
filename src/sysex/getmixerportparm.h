#ifndef GETMIXERPORTPARM_H
#define GETMIXERPORTPARM_H

#include "sysexmessage.h"

class GetMixerPortParm : public SysExMessage {
  public:
	GetMixerPortParm(Device *device);
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;

  protected:
	std::vector<unsigned char> *m_pGetMessageData() override;
};

#endif // GETMIXERPORTPARM_H
