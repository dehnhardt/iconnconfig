#ifndef GETMIXEROUTPUTCONTROL_H
#define GETMIXEROUTPUTCONTROL_H

#include "portsysexmessage.h"

class GetMixerOutputControl : public PortSysExMessage {
  public:
	GetMixerOutputControl(Device *device);

	virtual ~GetMixerOutputControl();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice);

  protected:
	std::vector<unsigned char> *getMessageData();
};

#endif // GETMIXEROUTPUTCONTROL_H
