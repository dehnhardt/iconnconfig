#ifndef GETMIXERINPUTPARM_H
#define GETMIXERINPUTPARM_H

#include "portsysexmessage.h"

class GetMixerOutputParm : public PortSysExMessage {
  public:
	GetMixerOutputParm(Device *device);

	// SysExMessage interface
	void setMixerInputNumber(unsigned int iMixerInputNumber);

  protected:
	std::vector<unsigned char> *getMessageData() override;
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice) override;

  private:
	unsigned int m_iMixerInputNumber = 0;
};

#endif // GETMIXERINPUTPARM_H
