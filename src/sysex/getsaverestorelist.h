#ifndef GETSAVERESTORELIST_H
#define GETSAVERESTORELIST_H

#include "sysexmessage.h"

class GetSaveRestoreList : public SysExMessage {
public:
	GetSaveRestoreList(Device *device);
	GetSaveRestoreList(SysExMessage::Command cmd, BYTE_VECTOR *message,
										 Device *device)
			: SysExMessage(cmd, message, device) {}

	void createAnswer(SysExMessage::Command cmd,
										std::vector<unsigned char> *message, Device *device);
};

#endif // GETSAVERESTORELIST_H
