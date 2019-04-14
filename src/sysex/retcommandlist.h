#ifndef COMMANDS_H
#define COMMANDS_H

#include "sysexmessage.h"

#include <vector>

class RetCommandList : public SysExMessage {
  public:
	RetCommandList(Device *m_pDevice);
	RetCommandList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	~RetCommandList() override;

	std::vector<int> getSupportedCommands();
	bool isCommandSupported(Command m_Command);
	void parseAnswerData() override;
	int getSettingsId() override { return m_Command; }
	int getSettingsIndex() override { return 0; }
	std::string getStorableValue() override { return ""; }

  private:
	std::vector<Command> *m_pSupportedCommands = nullptr;
};

#endif // GETCOMMANDS_H
