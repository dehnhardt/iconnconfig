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

std::vector<unsigned char> *PortSysExMessage::getPortIdBytes() {
	return MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
}
