#ifndef GETMIXERMETERVALUE_H
#define GETMIXERMETERVALUE_H

#include "portsysexmessage.h"

class GetMixerMeterValue : public PortSysExMessage {

  public:
	GetMixerMeterValue(Device *m_pDevice);
	~GetMixerMeterValue();
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
	                  Device *m_pDevice);

	void setPortDirections(pk::PortDirection direction) {
		m_portDirection = direction;
	}

	void setOutputNumber(unsigned int iOutputNumber) {
		m_iMixerOutoutNumber = iOutputNumber;
	}

  protected:
	std::vector<unsigned char> *getMessageData();

  private:
	unsigned int m_iMixerOutoutNumber = 2;
	pk::PortDirection m_portDirection = pk::PortDirection::BOTH;
};

#endif // GETMIXERMETERVALUE_H
