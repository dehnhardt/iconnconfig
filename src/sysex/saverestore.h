#ifndef SAVERESTORE_H
#define SAVERESTORE_H

#include "sysexmessage.h"

class SaveRestore : public SysExMessage {
public:
	enum SaveResstoreId {
		NONE = 0,
		SAVE_TO_DEVICE = 1,
		RESTORE_FROM_DEVICE = 2,
		SET_TO_FACTORY_DEFAULT = 3
	};

	SaveRestore(Device *device);
	SaveRestore(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	void setSaveRestoreId(SaveResstoreId saveRestoreId);
	BYTE_VECTOR *getMessageData();
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	SaveResstoreId saveRestoreId = NONE;
};

#endif// SAVERESTORE_H
