#ifndef RETSETMIDIPORTINFO_H
#define RETSETMIDIPORTINFO_H

#include "sysexmessage.h"

enum MidiPortType { NONE, DIN, USB_DEVICE, USB_HOST, ETHERNET };

class RetSetMidiPortInfo : public SysExMessage {
public:
	RetSetMidiPortInfo(Device *m_pDevice);

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
	int getPortNumberOfType() const;
	int getJackNumberOfType() const;
	std::string getPortName() const;

public:
	// setter
	void setPortName(const std::string &value);

public:
	// methods
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	long m_iPortId;
	MidiPortType m_PortType;
	bool m_bInputEnabled;
	bool m_bOutputEnabled;

	int m_iJackNumber = 0;
	int m_iUsbDevicePort = 0;
	int m_iUsbHostPort = 0;
	int m_iEthernetSession = 0;

	int m_iPortNameLength = 0;
	std::string m_sPortName;
	std::string m_sPortTypeName;
};

#endif// RETSETMIDIPORTINFO_H
