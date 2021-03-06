#include "getsaverestorelist.h"
#include "retsaverestorelist.h"

GetSaveRestoreList::GetSaveRestoreList(Device *device)
    : SysExMessage(Command::GET_SAVE_RESTORE_LIST, SysExMessage::QUERY,
                   device) {}

GetSaveRestoreList::~GetSaveRestoreList() {}

void GetSaveRestoreList::createAnswer(Command cmd, BYTE_VECTOR *message,
                                      Device *device) {
	m_pAnswer = std::make_shared<RetSaveRestoreList>(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
