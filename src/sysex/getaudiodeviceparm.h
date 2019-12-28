#ifndef GETAUDIODEVICEPARM_H
#define GETAUDIODEVICEPARM_H

#include "portsysexmessage.h"

class GetAudioDeviceParm : public PortSysExMessage {
  public:
	GetAudioDeviceParm(Device *device);

	virtual ~GetAudioDeviceParm();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

  protected:
	std::vector<unsigned char> *getMessageData();
};

#endif // GETAUDIODEVICEPARM_H
