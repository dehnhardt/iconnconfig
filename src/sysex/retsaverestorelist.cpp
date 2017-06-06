#include "retsaverestorelist.h"

RetSaveRestoreList::RetSaveRestoreList(Device *device)
		: SysExMessage(SysExMessage::RET_SAVE_RESTORE_LIST, SysExMessage::QUERY,
									 device) {}
