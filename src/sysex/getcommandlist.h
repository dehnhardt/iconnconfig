#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "retcommandlist.h"
#include "sysexmessage.h"

class GetCommandList : public SysExMessage {
public:
	GetCommandList(Device *m_pDevice);
	GetCommandList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		m_pAnswer = std::make_shared<RetCommandList>(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
};

#endif// GETCOMMANDS_H
