#include "retsetmidiportroute.h"

#include <cmath>

RetSetMidiPortRoute::RetSetMidiPortRoute(Device *device)
    : SysExMessage(Command::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
                   device) {}

RetSetMidiPortRoute::~RetSetMidiPortRoute() { delete m_pPortRoutings; }

void RetSetMidiPortRoute::parseAnswerData() {
	m_iPortId = MIDI::byteJoin7bit(m_pData, 1, 2);
	m_pPortRoutings = new BYTE_VECTOR(m_pData->begin() + 3, m_pData->end());
}

bool RetSetMidiPortRoute::isPortRouted(int portNumber) {
	int byte = -1;
	int bit = -1;
	getPortByteAndBit(portNumber, byte, bit);
	if (byte <= m_iNumerOfExpectedBytes) {
		unsigned int byteVal =
		    m_pPortRoutings->at(static_cast<unsigned long>(byte));
		unsigned int mask = static_cast<unsigned int>(pow(2, bit));
		return byteVal & mask;
	}
	return false;
}

void RetSetMidiPortRoute::setPortRouted(int portNumber, bool routed) {
	int byte = -1;
	int bit = -1;
	int val = 0;
	getPortByteAndBit(portNumber, byte, bit);
	if (byte != -1 && bit != -1)
		val = m_pPortRoutings->at(static_cast<unsigned long>(byte));
	unsigned int mask = static_cast<unsigned int>(pow(2, bit));
	if (routed) {
		val |= mask;
	} else {
		val &= ~mask;
	}
	(*m_pPortRoutings)[static_cast<unsigned long>(byte)] =
	    static_cast<unsigned char>(val);
}

int RetSetMidiPortRoute::getTotalNumberOfPorts() const {
	return m_iTotalNumberOfPorts;
}

void RetSetMidiPortRoute::setTotalNumberOfPorts(int value) {
	m_iTotalNumberOfPorts = value;
	m_iNumerOfExpectedBytes = getNumberOfExpectedBytes(m_iTotalNumberOfPorts);
}

std::vector<unsigned char> *RetSetMidiPortRoute::getMessageData() {
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back(0x01);
	BYTE_VECTOR *portIdV =
	    MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	messageData->insert(messageData->end(), portIdV->begin(), portIdV->end());
	messageData->insert(messageData->end(), m_pPortRoutings->begin(),
	                    m_pPortRoutings->end());
	return messageData;
}

void RetSetMidiPortRoute::getPortByteAndBit(int portNumber, int &byte,
                                            int &bit) {
	--portNumber;
	if (m_iNumerOfExpectedBytes != -1 &&
	    static_cast<unsigned int>(m_iNumerOfExpectedBytes) >=
	        m_pPortRoutings->size())
		byte = portNumber / 4;
	bit = portNumber - (byte * 4);
}

int RetSetMidiPortRoute::getNumberOfExpectedBytes(int totalPortNumber) {
	return (((totalPortNumber - 1) / 8) + 1) * 2;
}
