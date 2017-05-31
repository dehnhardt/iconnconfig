#include "retsetmidiportinfo.h"

RetSetMidiPortInfo::RetSetMidiPortInfo(Device *device)
		: SysExMessage(SysExMessage::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
									 device) {}

void RetSetMidiPortInfo::parseAnswerData() {
	portId = MIDI::byteJoin(data, 1, 2);
	portType = (MidiPortType)data->at(3);
	jackNumber = data->at(4);

	switch (portType) {
	case NONE:
		break;
	case DIN:
		portTypeName = "DIN-Port";
		break;
	case USB_DEVICE:
		usbDevicePort = data->at(5);
		portTypeName = "USB-Device-Port";
		break;
	case USB_HOST:
		usbHostPort = data->at(5);
		portTypeName = "USB-Host-Port";
		break;
	case ETHERNET:
		ethernetSession = data->at(5);
		portTypeName = "Ethernet-Port";
		break;
	}
	portNameLength = data->at(8);
	int portFlags = data->at(9);
	inputEnabled = ((portFlags & 0x01) == 0x01);
	outputEnabled = ((portFlags & 0x02) == 0x02);
	portName = std::string(data->begin() + 10, data->end());
}

long RetSetMidiPortInfo::getPortId() const { return portId; }

MidiPortType RetSetMidiPortInfo::getPortType() const { return portType; }

bool RetSetMidiPortInfo::getInputEnabled() const { return inputEnabled; }

bool RetSetMidiPortInfo::getOutputEnabled() const { return outputEnabled; }

int RetSetMidiPortInfo::getJackNumber() const { return jackNumber; }

int RetSetMidiPortInfo::getUsbDevicePort() const { return usbDevicePort; }

int RetSetMidiPortInfo::getUsbHostPort() const { return usbHostPort; }

void RetSetMidiPortInfo::setUsbHostPort(int value) { usbHostPort = value; }

int RetSetMidiPortInfo::getEthernetSession() const { return ethernetSession; }

int RetSetMidiPortInfo::getPortNumberOfType() const {
	switch (portType) {
	case DIN:
		return jackNumber;
	case USB_DEVICE:
		return usbDevicePort;
	case USB_HOST:
		return usbHostPort;
	case ETHERNET:
		return ethernetSession;
	case NONE:
		return 0;
	}
	return 0;
}

std::string RetSetMidiPortInfo::getPortName() const { return portName; }

void RetSetMidiPortInfo::setPortName(const std::string &value) {
	portName = value;
}
