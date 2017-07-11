#ifndef RETSETSAVERESTORELIST_H
#define RETSETSAVERESTORELIST_H

#include "sysexmessage.h"

class RetSaveRestoreList : public SysExMessage {
public:
	RetSaveRestoreList(Device *device);
	RetSaveRestoreList(SysExMessage::Command cmd, BYTE_VECTOR *message,
					   Device *device)
		: SysExMessage(cmd, message, device) {}
	BYTE_VECTOR *getSaveRestoreList() { return data; }
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
};

#endif// RETSETSAVERESTORELIST_H
