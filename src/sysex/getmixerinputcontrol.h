#ifndef GETMIXERINPUTCONTROL_H
#define GETMIXERINPUTCONTROL_H

#include "portsysexmessage.h"

class GetMixerInputControl : public PortSysExMessage {
  public:
	GetMixerInputControl(Device *device);

	virtual ~GetMixerInputControl();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

  protected:
	std::vector<unsigned char> *getMessageData();
};

#endif // GETMIXERINPUTCONTROL_H
