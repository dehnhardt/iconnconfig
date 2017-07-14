#include "getsaverestorelist.h"
#include "retsaverestorelist.h"

GetSaveRestoreList::GetSaveRestoreList(Device *device)
	: SysExMessage(Command::GET_SAVE_RESTORE_LIST, SysExMessage::QUERY,
				   device) {}

void GetSaveRestoreList::createAnswer(Command cmd, BYTE_VECTOR *message,
									  Device *device) {
	answer = new RetSaveRestoreList(cmd, message, device);
	if (debug)
		answer->setDebug(true);
	answer->parseAnswerData();
}
