#ifndef GETAUDIOPORTMETERVALUE_H
#define GETAUDIOPORTMETERVALUE_H

#include "portsysexmessage.h"

class GetAudioPortMeterValue : public PortSysExMessage {

  public:
	GetAudioPortMeterValue(Device *m_pDevice);
	~GetAudioPortMeterValue();
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);

	void setPortDirections(pk::PortDirection direction) {
		m_portDirection = direction;
	}

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	pk::PortDirection m_portDirection = pk::PortDirection::BOTH;
};

#endif // GETAUDIOPORTMETERVALUE_H
