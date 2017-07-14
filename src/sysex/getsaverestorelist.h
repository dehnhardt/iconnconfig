#ifndef GETSAVERESTORELIST_H
#define GETSAVERESTORELIST_H

#include "sysexmessage.h"

class GetSaveRestoreList : public SysExMessage {
public:
	GetSaveRestoreList(Device *device);
	GetSaveRestoreList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	void createAnswer(Command cmd, std::vector<unsigned char> *message,
					  Device *device);
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
};

#endif// GETSAVERESTORELIST_H
