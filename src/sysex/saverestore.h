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

	SaveRestore(Device *m_pDevice);
	SaveRestore(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

	void setSaveRestoreId(SaveResstoreId m_SaveRestoreId);
	BYTE_VECTOR *m_pGetMessageData();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	SaveResstoreId m_SaveRestoreId = NONE;
};

#endif// SAVERESTORE_H
