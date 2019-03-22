#ifndef GETAUDIOPORTMETERVALUE_H
#define GETAUDIOPORTMETERVALUE_H

#include "portsysexmessage.h"

class GetAudioPortMeterValue : public PortSysExMessage {

  public:
	GetAudioPortMeterValue(Device *m_pDevice);
	~GetAudioPortMeterValue();
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

	void setPortDirections(PortDirection direction) {
		m_portDirection = direction;
	}

  protected:
	std::vector<unsigned char> *m_pGetMessageData();

  private:
	PortDirection m_portDirection = BOTH;
};

#endif // GETAUDIOPORTMETERVALUE_H
