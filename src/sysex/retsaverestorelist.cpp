#include "retsaverestorelist.h"

RetSaveRestoreList::RetSaveRestoreList(Device *device)
	: SysExMessage(Command::RET_SAVE_RESTORE_LIST, SysExMessage::QUERY,
				   device) {}
