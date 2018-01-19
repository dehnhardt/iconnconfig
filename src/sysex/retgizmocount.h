#ifndef RETGIZMOCOUNT_H
#define RETGIZMOCOUNT_H

#include "sysexmessage.h"

class RetGizmoCount : public SysExMessage {
public:
	RetGizmoCount(Device *m_pDevice);
	RetGizmoCount(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~RetGizmoCount() {}
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	unsigned int m_iGizmoCount = 0;
	void parseAnswerData();
};

#endif// RETGIZMOCOUNT_H
