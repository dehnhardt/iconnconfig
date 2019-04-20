#include "portsysexmessage.h"

PortSysExMessage::PortSysExMessage(Command m_Command,
								   SysExMessage::CommandFlags flags,
								   Device *m_pDevice)
	: SysExMessage(m_Command, flags, m_pDevice) {}

PortSysExMessage::PortSysExMessage(Command m_Command,
								   std::vector<unsigned char> *message,
								   Device *m_pDevice)
	: SysExMessage(m_Command, message, m_pDevice) {}

PortSysExMessage::~PortSysExMessage() {}

unsigned int PortSysExMessage::parsePortId(unsigned long offset) {
	return static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, offset, 2));
}

std::vector<unsigned char> *PortSysExMessage::getPortIdBytes() {
	return MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
}
