#ifndef GETETHERNETPORTINFO_H
#define GETETHERNETPORTINFO_H

#include "sysexmessage.h"

class GetEthernetPortInfo : public SysExMessage {
public:
	GetEthernetPortInfo(Device *m_pDevice);
	GetEthernetPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	std::vector<unsigned char> *m_pGetMessageData();
};

#endif// GETETHERNETPORTINFO_H
