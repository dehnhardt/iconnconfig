#ifndef RETSETMIDIINFO_H
#define RETSETMIDIINFO_H

#include "sysexmessage.h"

class RetSetMidiInfo : public SysExMessage {
public:
	RetSetMidiInfo(Device *m_pDevice);

	RetSetMidiInfo(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}

public:
	// getters
	int getMidiPorts() const;
	int getCurrentCommunicationPort() const;
	int getDinPorts() const;
	int getUsbDeviceJacks() const;
	int getUsbHostJacks() const;
	int getEthernetJacks() const;
	int getUsbMidiDevicePorts() const;
	int getUsbMidiHostPorts() const;
	int getRtpMidiSessions() const;
	int getRtpMidiConnections() const;
	int getMaximumSupportedUsbHostPorts() const;
	bool getEnableRoutingBetweenHostJacks() const;
	bool getEnableRunningStatusOnDin() const;

public:
	// setter
	void setEnableRunningStatusOnDin(bool value);
	void setEnableRoutingBetweenHostJacks(bool value);
	void setMaximumSupportedUsbHostPorts(int value);

public:
	// methods
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	void parseAnswerData();

private:
	int m_iMidiPorts = 0;
	int m_iCurrentCommunicationPort = 0;
	int m_iDinPorts = 0;
	int m_iUsbDeviceJacks = 0;
	int m_iUsbHostJacks = 0;
	int m_iEthernetJacks = 0;
	int m_iUsbMidiDevicePorts = 0;
	int m_iUsbMidiHostPorts = 0;
	int m_iRtpMidiSessions = 0;
	int m_iRtpMidiConnections = 0;
	int m_iMaximumSupportedUsbHostPorts = 0;

	bool m_bEnableRoutingBetweenHostJacks = false;
	bool m_bEnableRunningStatusOnDin = false;
};

#endif// RETSETMIDIINFO_H
