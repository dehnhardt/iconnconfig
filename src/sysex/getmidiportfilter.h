#ifndef GETMIDIPORTFILTER_H
#define GETMIDIPORTFILTER_H

#include "retsetmidiportfilter.h"
#include "sysexmessage.h"

class GetMidiPortFilter : public SysExMessage {

  public:
  public:
	GetMidiPortFilter(Device *device);

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

  public:
	// setter
	void setPortNumer(unsigned int portNumber) {
		this->m_iPortNumber = portNumber;
	}
	void setPortFilterDirection(PortDirection portFilterDirection) {
		this->m_iPortFilterDirection = portFilterDirection;
	}

  protected:
	std::vector<unsigned char> *m_pGetMessageData();

  private:
	unsigned int m_iPortNumber;
	PortDirection m_iPortFilterDirection;
};

#endif // GETMIDIPORTFILTER_H
