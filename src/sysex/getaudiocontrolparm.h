#ifndef GETAUDIOCONTROLPARM_H
#define GETAUDIOCONTROLPARM_H

#include "portsysexmessage.h"

class GetAudioControlParm : public PortSysExMessage {

  public:
	GetAudioControlParm(Device *m_pDevice);
	virtual ~GetAudioControlParm();

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice);

	unsigned int getControllerNumber() const;
	void setControllerNumber(unsigned int iControllerNumber);

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	unsigned int m_iControllerNumber = 0;
};

#endif // GETAUDIOCONTROLPARM_H
