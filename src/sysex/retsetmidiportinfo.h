#ifndef RETSETMIDIPORTINFO_H
#define RETSETMIDIPORTINFO_H

#include "sysexmessage.h"

enum MidiPortType { NONE, DIN, USB_DEVICE, USB_HOST, ETHERNET };

class RetSetMidiPortInfo : public SysExMessage {
public:
	RetSetMidiPortInfo(Device *device);

	RetSetMidiPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}

	void parseAnswerData();

public:
	// getter
	long getPortId() const;
	MidiPortType getPortType() const;
	bool getInputEnabled() const;
	bool getOutputEnabled() const;
	int getJackNumber() const;
	int getUsbDevicePort() const;
	int getUsbHostPort() const;
	void setUsbHostPort(int value);
	int getEthernetSession() const;
	int getPortNumerOfType() const;
	std::string getPortName() const;

public:
	// setter
	void setPortName(const std::string &value);

private:
	long portId;
	MidiPortType portType;
	bool inputEnabled;
	bool outputEnabled;

	int jackNumber = 0;
	int usbDevicePort = 0;
	int usbHostPort = 0;
	int ethernetSession = 0;

	int portNameLength = 0;
	std::string portName;
	std::string portTypeName;
};

#endif // RETSETMIDIPORTINFO_H
