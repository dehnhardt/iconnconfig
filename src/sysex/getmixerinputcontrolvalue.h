#ifndef GETMIXERINPUTCONTROLVALUE_H
#define GETMIXERINPUTCONTROLVALUE_H

#include "portsysexmessage.h"

class GetMixerInputControlValue : public PortSysExMessage {
  public:
	GetMixerInputControlValue(Device *device);
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

	void setMixerOutputNumber(unsigned int iMixerOutputNumber);

	void setMixerInputNumber(unsigned int iMixerInputNumber);

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	unsigned int m_iMixerOutputNumber = 0;
	unsigned int m_iMixerInputNumber = 0;
};

#endif // GETMIXERINPUTCONTROLVALUE_H
