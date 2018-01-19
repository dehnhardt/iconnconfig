#include "retcommandlist.h"
#include "midi.h"

RetCommandList::RetCommandList(Device *device)
	: SysExMessage(Command::RET_COMMAND_LIST, SysExMessage::QUERY, device) {}

bool RetCommandList::isCommandSupported(const Command cmd) {
	if (!m_pSupportedCommands)
		return false;
	std::vector<Command>::iterator it;
	for (it = m_pSupportedCommands->begin(); it != m_pSupportedCommands->end();
		 it++) {
		if (*it == cmd)
			return true;
	}
	return false;
}

void RetCommandList::parseAnswerData() {
	m_pSupportedCommands = new std::vector<Command>();
	unsigned int nCommandsSize = m_pData->size();
	Command c;
	for (unsigned int i = 0; i < nCommandsSize; ++i) {
		int val = m_pData->at(i);
		c = static_cast<Command>(val);
		if ((val != 0) && (val < 0x40))
			m_pSupportedCommands->push_back(c);
	}
}
