#ifndef RETGIZMOCOUNT_H
#define RETGIZMOCOUNT_H

#include "sysexmessage.h"

class RetGizmoCount : public SysExMessage {
public:
	RetGizmoCount(Device *device);
	RetGizmoCount(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~RetGizmoCount() {}
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	unsigned int gizmoCount = 0;
	void parseAnswerData();
};

#endif// RETGIZMOCOUNT_H
