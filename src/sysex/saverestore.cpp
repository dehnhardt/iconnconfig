#include "saverestore.h"

SaveRestore::SaveRestore(Device *device)
	: SysExMessage(Command::SAVE_RESTORE, SysExMessage::QUERY, device) {}

void SaveRestore::setSaveRestoreId(SaveResstoreId saveRestoreId) {
	this->saveRestoreId = saveRestoreId;
}

BYTE_VECTOR *SaveRestore::getMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back((int)this->saveRestoreId);
	return messageData;
}
