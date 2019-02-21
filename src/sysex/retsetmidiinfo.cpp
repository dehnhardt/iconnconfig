#include "retsetmidiinfo.h"

RetSetMidiInfo::RetSetMidiInfo(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_INFO, SysExMessage::QUERY, device) {}

void RetSetMidiInfo::parseAnswerData() {
	if (m_pData->size() < 15)
		return;
	m_iCommandVersionNumber = this->m_pData->at(0);
	m_iMidiPorts = static_cast<int>(MIDI::byteJoin7bit(this->m_pData, 1, 2));
	m_iCurrentCommunicationPort = static_cast<int>(MIDI::byteJoin7bit(m_pData, 3, 2));
	m_iDinPorts = this->m_pData->at(5);
	m_iUsbDeviceJacks = this->m_pData->at(6);
	m_iUsbHostJacks = this->m_pData->at(7);
	m_iEthernetJacks = this->m_pData->at(8);
	m_iUsbMidiDevicePorts = this->m_pData->at(9);
	m_iUsbMidiHostPorts = this->m_pData->at(10);
	m_iRtpMidiSessions = this->m_pData->at(11);
	m_iRtpMidiConnections = this->m_pData->at(12);
	int flags = this->m_pData->at(13);
	m_bEnableRoutingBetweenHostJacks = ((flags & 1) == 1);
	m_bEnableRunningStatusOnDin = ((flags & 2) == 2);
	m_iMaximumSupportedUsbHostPorts = this->m_pData->at(14);
}

void RetSetMidiInfo::setMaximumSupportedUsbHostPorts(int value) {
	m_iMaximumSupportedUsbHostPorts = value;
}

bool RetSetMidiInfo::getEnableRunningStatusOnDin() const {
	return m_bEnableRunningStatusOnDin;
}

void RetSetMidiInfo::setEnableRunningStatusOnDin(bool value) {
	m_bEnableRunningStatusOnDin = value;
}

bool RetSetMidiInfo::getEnableRoutingBetweenHostJacks() const {
	return m_bEnableRoutingBetweenHostJacks;
}

void RetSetMidiInfo::setEnableRoutingBetweenHostJacks(bool value) {
	m_bEnableRoutingBetweenHostJacks = value;
}

int RetSetMidiInfo::getMaximumSupportedUsbHostPorts() const {
	return m_iMaximumSupportedUsbHostPorts;
}

int RetSetMidiInfo::getRtpMidiConnections() const { return m_iRtpMidiConnections; }

int RetSetMidiInfo::getRtpMidiSessions() const { return m_iRtpMidiSessions; }

int RetSetMidiInfo::getUsbMidiHostPorts() const { return m_iUsbMidiHostPorts; }

int RetSetMidiInfo::getUsbMidiDevicePorts() const { return m_iUsbMidiDevicePorts; }

int RetSetMidiInfo::getEthernetJacks() const { return m_iEthernetJacks; }

int RetSetMidiInfo::getUsbHostJacks() const { return m_iUsbHostJacks; }

int RetSetMidiInfo::getUsbDeviceJacks() const { return m_iUsbDeviceJacks; }

int RetSetMidiInfo::getDinPorts() const { return m_iDinPorts; }

int RetSetMidiInfo::getCurrentCommunicationPort() const {
	return m_iCurrentCommunicationPort;
}

int RetSetMidiInfo::getMidiPorts() const { return m_iMidiPorts; }
