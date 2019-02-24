#ifndef GETMIDIPORTREMAP_H
#define GETMIDIPORTREMAP_H

#include "retsetmidiportremap.h"
#include "sysexmessage.h"

class GetMidiPortRemap : public SysExMessage {

  public:
  public:
    GetMidiPortRemap(Device *device);

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
    void setPortRemapDirection(PortRemapDirection portRemapDirection) {
        this->m_iPortRemapDirection = portRemapDirection;
	}

  protected:
	std::vector<unsigned char> *m_pGetMessageData();

  private:
	unsigned int m_iPortNumber;
    PortRemapDirection m_iPortRemapDirection;
};

#endif // GETMIDIPORTREMAP_H
