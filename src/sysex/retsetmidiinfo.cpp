#include "retsetmidiinfo.h"

RetSetMidiInfo::RetSetMidiInfo(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_INFO, SysExMessage::QUERY, device) {}

void RetSetMidiInfo::parseAnswerData() {
	if (data->size() < 15)
		return;
	commandVersionNumber = this->data->at(0);
	midiPorts = static_cast<int>(MIDI::byteJoin(this->data, 1, 2));
	currentCommunicationPort = static_cast<int>(MIDI::byteJoin(data, 3, 2));
	dinPorts = this->data->at(5);
	usbDeviceJacks = this->data->at(6);
	usbHostJacks = this->data->at(7);
	ethernetJacks = this->data->at(8);
	usbMidiDevicePorts = this->data->at(9);
	usbMidiHostPorts = this->data->at(10);
	rtpMidiSessions = this->data->at(11);
	rtpMidiConnections = this->data->at(12);
	int flags = this->data->at(13);
	enableRoutingBetweenHostJacks = ((flags & 1) == 1);
	enableRunningStatusOnDin = ((flags & 2) == 2);
	maximumSupportedUsbHostPorts = this->data->at(14);
}

void RetSetMidiInfo::setMaximumSupportedUsbHostPorts(int value) {
	maximumSupportedUsbHostPorts = value;
}

bool RetSetMidiInfo::getEnableRunningStatusOnDin() const {
	return enableRunningStatusOnDin;
}

void RetSetMidiInfo::setEnableRunningStatusOnDin(bool value) {
	enableRunningStatusOnDin = value;
}

bool RetSetMidiInfo::getEnableRoutingBetweenHostJacks() const {
	return enableRoutingBetweenHostJacks;
}

void RetSetMidiInfo::setEnableRoutingBetweenHostJacks(bool value) {
	enableRoutingBetweenHostJacks = value;
}

int RetSetMidiInfo::getMaximumSupportedUsbHostPorts() const {
	return maximumSupportedUsbHostPorts;
}

int RetSetMidiInfo::getRtpMidiConnections() const { return rtpMidiConnections; }

int RetSetMidiInfo::getRtpMidiSessions() const { return rtpMidiSessions; }

int RetSetMidiInfo::getUsbMidiHostPorts() const { return usbMidiHostPorts; }

int RetSetMidiInfo::getUsbMidiDevicePorts() const { return usbMidiDevicePorts; }

int RetSetMidiInfo::getEthernetJacks() const { return ethernetJacks; }

int RetSetMidiInfo::getUsbHostJacks() const { return usbHostJacks; }

int RetSetMidiInfo::getUsbDeviceJacks() const { return usbDeviceJacks; }

int RetSetMidiInfo::getDinPorts() const { return dinPorts; }

int RetSetMidiInfo::getCurrentCommunicationPort() const {
	return currentCommunicationPort;
}

int RetSetMidiInfo::getMidiPorts() const { return midiPorts; }
