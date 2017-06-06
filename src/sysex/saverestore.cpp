#include "saverestore.h"

SaveRestore::SaveRestore(Device *device)
		: SysExMessage(SysExMessage::SAVE_RESTORE, SysExMessage::QUERY, device) {}
