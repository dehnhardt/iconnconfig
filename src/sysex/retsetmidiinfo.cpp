#include "retsetmidiinfo.h"

RetSetMidiInfo::RetSetMidiInfo(Device *device)
		: SysExMessage(SysExMessage::RET_SET_MIDI_INFO, SysExMessage::QUERY,
									 device) {}

void RetSetMidiInfo::parseAnswerData() {
	if (data->size() < 15)
		return;
	commandVersionNumber = this->data->at(0);
	midiPorts = MIDI::byteJoin(this->data, 1, 2);
	currentCommunicationPort = MIDI::byteJoin(data, 3, 4);
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
