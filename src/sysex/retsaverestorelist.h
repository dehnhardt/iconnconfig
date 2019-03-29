#ifndef RETSETSAVERESTORELIST_H
#define RETSETSAVERESTORELIST_H

#include "sysexmessage.h"

class RetSaveRestoreList : public SysExMessage {
  public:
	RetSaveRestoreList(Device *m_pDevice);
	RetSaveRestoreList(Command cmd, BYTE_VECTOR *message, Device *device)
	    : SysExMessage(cmd, message, device) {}
	~RetSaveRestoreList();
	BYTE_VECTOR *getSaveRestoreList() { return new BYTE_VECTOR(*m_pData); }
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
};

#endif // RETSETSAVERESTORELIST_H
