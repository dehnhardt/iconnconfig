#ifndef COMMANDS_H
#define COMMANDS_H

#include "sysexmessage.h"

#include <vector>

class RetCommandList : public SysExMessage {
public:
	RetCommandList(Device *m_pDevice);
	RetCommandList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	std::vector<int> getSupportedCommands();
	bool isCommandSupported(Command m_Command);
	void parseAnswerData();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	std::vector<Command> *m_pSupportedCommands = 0;
};

#endif// GETCOMMANDS_H
