#ifndef GETGIZMOCOUNT_H
#define GETGIZMOCOUNT_H

#include "retgizmocount.h"
#include "sysexmessage.h"

class GetGizmoCount : public SysExMessage {
public:
	GetGizmoCount(Device *m_pDevice);
	GetGizmoCount(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~GetGizmoCount() {}
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		m_pAnswer = new RetGizmoCount(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
};

#endif// GETGIZMOCOUNT_H
