#ifndef GETMIXEROUTPUTPARM_H
#define GETMIXEROUTPUTPARM_H

#include "portsysexmessage.h"

class GetMixerOutputParm : public PortSysExMessage {
  public:
	GetMixerOutputParm(Device *device);

	// SysExMessage interface
	void setMixerOutputNumber(unsigned int iMixerOutputNumber);

  protected:
	std::vector<unsigned char> *getMessageData() override;
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;

  private:
	unsigned int m_iMixerOutputNumber = 0;
};

#endif // GETMIXEROUTPUTPARM_H
