#ifndef SAVERESTORE_H
#define SAVERESTORE_H

#include "sysexmessage.h"

class SaveRestore : public SysExMessage {
public:
	SaveRestore(Device *device);
	SaveRestore(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}
};

#endif // SAVERESTORE_H
