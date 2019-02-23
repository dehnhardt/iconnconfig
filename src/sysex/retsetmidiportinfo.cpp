#include "retsetmidiportinfo.h"

RetSetMidiPortInfo::RetSetMidiPortInfo(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
				   device) {}

void RetSetMidiPortInfo::parseAnswerData() {
	m_iPortId = MIDI::byteJoin7bit(m_pData, 1, 2);
	m_PortType = static_cast<MidiPortType>(m_pData->at(3));
	m_iJackNumber = m_pData->at(4);

	switch (m_PortType) {
	case NONE:
		break;
	case DIN:
		m_sPortTypeName = "DIN-Port";
		break;
	case USB_DEVICE:
		m_iUsbDevicePort = m_pData->at(5);
		m_sPortTypeName = "USB-Device-Port";
		break;
	case USB_HOST:
		m_iUsbHostPort = m_pData->at(5);
		m_sPortTypeName = "USB-Host-Port";
		break;
	case ETHERNET:
		m_iEthernetSession = m_pData->at(5);
		m_sPortTypeName = "Ethernet-Port";
		break;
	}
	m_iPortNameLength = m_pData->at(8);
	m_bPortNameWritable = m_iPortNameLength > 0;
	int portFlags = m_pData->at(9);
	m_bInputEnabled = ((portFlags & 0x01) == 0x01);
	m_bOutputEnabled = ((portFlags & 0x02) == 0x02);
	m_sPortName = std::string(m_pData->begin() + 10, m_pData->end());
}

std::vector<unsigned char> *RetSetMidiPortInfo::m_pGetMessageData() {
	// Write Flag
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portIdV =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), portIdV->begin(), portIdV->end());
	data->push_back(m_PortType);
	data->push_back(static_cast<unsigned char>(m_iJackNumber));
	switch (m_PortType) {
	case NONE:
		break;
	case DIN:
		data->push_back(0);
		data->push_back(0);
		data->push_back(0);
		break;
	case USB_DEVICE:
		data->push_back(m_iUsbDevicePort);
		data->push_back(0);
		data->push_back(0);
		break;
	case USB_HOST:
		data->push_back(m_iUsbHostPort);
		data->push_back(0);
		data->push_back(0);
		break;
	case ETHERNET:
		data->push_back(m_iEthernetSession);
		data->push_back(0);
		data->push_back(0);
		break;
	}
	if (!m_bPortNameWritable)
		data->push_back(0);
	else
		data->push_back(static_cast<unsigned char>(m_iPortNameLength));
	unsigned char portFlags = 0;
	if (m_bInputEnabled)
		portFlags += 1;
	if (m_bOutputEnabled)
		portFlags += 2;
	data->push_back(portFlags);
	if (m_bPortNameWritable) {
		data->insert(data->end(), m_sPortName.begin(), m_sPortName.end());
	}
	return data;
}

long RetSetMidiPortInfo::getPortId() const { return m_iPortId; }

MidiPortType RetSetMidiPortInfo::getPortType() const { return m_PortType; }

bool RetSetMidiPortInfo::getInputEnabled() const { return m_bInputEnabled; }

bool RetSetMidiPortInfo::getOutputEnabled() const { return m_bOutputEnabled; }

bool RetSetMidiPortInfo::getPortNameWritable() const {
	return m_bPortNameWritable;
}

int RetSetMidiPortInfo::getJackNumber() const { return m_iJackNumber; }

int RetSetMidiPortInfo::getUsbDevicePort() const { return m_iUsbDevicePort; }

int RetSetMidiPortInfo::getUsbHostPort() const { return m_iUsbHostPort; }

void RetSetMidiPortInfo::setUsbHostPort(unsigned char value) {
	m_iUsbHostPort = value;
}

int RetSetMidiPortInfo::getEthernetSession() const {
	return m_iEthernetSession;
}

int RetSetMidiPortInfo::getPortNumberOfType() const {
	switch (m_PortType) {
	case DIN:
		return m_iJackNumber;
	case USB_DEVICE:
		return m_iUsbDevicePort;
	case USB_HOST:
		return m_iUsbHostPort;
	case ETHERNET:
		return m_iEthernetSession;
	case NONE:
		return 0;
	}
	return 0;
}

int RetSetMidiPortInfo::getJackNumberOfType() const {
	switch (m_PortType) {
	case DIN:
		return 0;
	case USB_DEVICE:
		return m_iJackNumber;
	case USB_HOST:
		return m_iJackNumber;
	case ETHERNET:
		return m_iJackNumber;
	case NONE:
		return 0;
	}
	return 0;
}

std::string RetSetMidiPortInfo::getPortName() const { return m_sPortName; }

void RetSetMidiPortInfo::setInputEnabled(bool enabled) {
	this->m_bInputEnabled = enabled;
}

void RetSetMidiPortInfo::setoutputEnabled(bool enabled) {
	m_bOutputEnabled = enabled;
}

void RetSetMidiPortInfo::setPortName(const std::string &value) {
	m_sPortName = value;
}
