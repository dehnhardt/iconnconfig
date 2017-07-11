#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "retcommandlist.h"
#include "sysexmessage.h"

class GetCommandList : public SysExMessage {
public:
	GetCommandList(Device *device);
	GetCommandList(SysExMessage::Command cmd, BYTE_VECTOR *message,
				   Device *device)
		: SysExMessage(cmd, message, device) {}
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
					  Device *device) {
		answer = new RetCommandList(cmd, message, device);
		answer->parseAnswerData();
	}
};

#endif// GETCOMMANDS_H
