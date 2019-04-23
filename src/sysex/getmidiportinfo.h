#ifndef GETMIDIPORTINFO_H
#define GETMIDIPORTINFO_H

#include "retsetmidiportinfo.h"
#include "sysexmessage.h"

class GetMidiPortInfo : public SysExMessage {
public:
	GetMidiPortInfo(Device *m_pDevice);
	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

public:
	// setter
	void setPortNumer(int portNumber) { this->m_iPortNumber = portNumber; }

protected:
	std::vector<unsigned char> *getMessageData();

private:
	int m_iPortNumber;
};

#endif// GETMIDIPORTINFO_H
