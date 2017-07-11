#include "retcommandlist.h"
#include "midi.h"

RetCommandList::RetCommandList(Device *device)
	: SysExMessage(SysExMessage::RET_COMMAND_LIST, SysExMessage::QUERY,
				   device) {}

bool RetCommandList::isCommandSupported(const Command cmd) {
	if (!supportedCommands)
		return false;
	std::vector<Command>::iterator it;
	for (it = supportedCommands->begin(); it != supportedCommands->end();
		 it++) {
		if (*it == cmd)
			return true;
	}
	return false;
}

void RetCommandList::parseAnswerData() {
	supportedCommands = new std::vector<Command>();
	unsigned int nCommandsSize = data->size();
	Command c;
	for (unsigned int i = 0; i < nCommandsSize; ++i) {
		int val = data->at(i);
		c = static_cast<Command>(val);
		if ((val != 0) && (val < 0x40))
			supportedCommands->push_back(c);
	}
}
