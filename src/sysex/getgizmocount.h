#ifndef GETGIZMOCOUNT_H
#define GETGIZMOCOUNT_H

#include "retgizmocount.h"
#include "sysexmessage.h"

class GetGizmoCount : public SysExMessage {
public:
	GetGizmoCount(Device *device);
	GetGizmoCount(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~GetGizmoCount() {}
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		answer = new RetGizmoCount(cmd, message, device);
		answer->parseAnswerData();
	}
};

#endif// GETGIZMOCOUNT_H
