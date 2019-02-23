#ifndef RETSETMIDIPORTINFO_H
#define RETSETMIDIPORTINFO_H

#include "sysexmessage.h"

enum MidiPortType { NONE, DIN, USB_DEVICE, USB_HOST, ETHERNET };

class RetSetMidiPortInfo : public SysExMessage {
  public:
	RetSetMidiPortInfo(Device *m_pDevice);

	RetSetMidiPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
	    : SysExMessage(cmd, message, device) {}

	void parseAnswerData() override;

  public:
	// getter
	long getPortId() const;
	MidiPortType getPortType() const;
	bool getInputEnabled() const;
	bool getOutputEnabled() const;
	bool getPortNameWritable() const;
	int getJackNumber() const;
	int getUsbDevicePort() const;
	int getUsbHostPort() const;
	void setUsbHostPort(unsigned char value);
	int getEthernetSession() const;
	int getPortNumberOfType() const;
	int getJackNumberOfType() const;
	std::string getPortName() const;

  public:
	// setter
	void setPortName(const std::string &value);
	void setInputEnabled(bool enabled);
	void setoutputEnabled(bool enabled);

  public:
	// methods
	int getSettingsId() override { return m_Command; }
	int getSettingsIndex() override { return 0; }
	std::string getStorableValue() override { return ""; }
	// SysExMessage interface
	std::vector<unsigned char> *m_pGetMessageData() override;

  private:
	long m_iPortId;
	MidiPortType m_PortType;
	bool m_bInputEnabled;
	bool m_bOutputEnabled;

	unsigned char m_iJackNumber = 0;
	unsigned char m_iUsbDevicePort = 0;
	unsigned char m_iUsbHostPort = 0;
	unsigned char m_iEthernetSession = 0;

	int m_iPortNameLength = 0;
	bool m_bPortNameWritable = 0;
	std::string m_sPortName;
	std::string m_sPortTypeName;
};

#endif // RETSETMIDIPORTINFO_H
