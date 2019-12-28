#ifndef GETAUDIOPATCHBAYPARM_H
#define GETAUDIOPATCHBAYPARM_H

#include "portsysexmessage.h"

class GetAudioPatchbayParm : public PortSysExMessage {
  public:
	GetAudioPatchbayParm(Device *device);
	virtual ~GetAudioPatchbayParm();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

  protected:
	std::vector<unsigned char> *getMessageData();
};

#endif // GETAUDIOPATCHBAYPARM_H
