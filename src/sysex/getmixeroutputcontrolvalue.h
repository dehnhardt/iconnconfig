#ifndef GETMIXEROUTPUTCONTROLVALUE_H
#define GETMIXEROUTPUTCONTROLVALUE_H

#include "portsysexmessage.h"

class GetMixerOutputControlValue : public PortSysExMessage {
  public:
	GetMixerOutputControlValue(Device *device);
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

	void setMixerOutputNumber(unsigned int iMixerOutputNumber);

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	unsigned int m_iMixerOutputNumber = 0;
};

#endif // GETMIXEROUTPUTCONTROLVALUE_H
