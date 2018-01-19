#include "saverestore.h"

SaveRestore::SaveRestore(Device *device)
	: SysExMessage(Command::SAVE_RESTORE, SysExMessage::QUERY, device) {}

void SaveRestore::setSaveRestoreId(SaveResstoreId saveRestoreId) {
	this->m_SaveRestoreId = saveRestoreId;
}

BYTE_VECTOR *SaveRestore::m_pGetMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back((int)this->m_SaveRestoreId);
	return messageData;
}
