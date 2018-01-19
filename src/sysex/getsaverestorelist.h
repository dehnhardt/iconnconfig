#ifndef GETSAVERESTORELIST_H
#define GETSAVERESTORELIST_H

#include "sysexmessage.h"

class GetSaveRestoreList : public SysExMessage {
public:
	GetSaveRestoreList(Device *m_pDevice);
	GetSaveRestoreList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	void createAnswer(Command m_Command, std::vector<unsigned char> *message,
					  Device *m_pDevice);
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
};

#endif// GETSAVERESTORELIST_H
