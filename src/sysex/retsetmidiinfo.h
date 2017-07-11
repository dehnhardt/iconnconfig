#ifndef RETSETMIDIINFO_H
#define RETSETMIDIINFO_H

#include "sysexmessage.h"

class RetSetMidiInfo : public SysExMessage {
public:
	RetSetMidiInfo(Device *device);

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
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	void parseAnswerData();

private:
	int midiPorts = 0;
	int currentCommunicationPort = 0;
	int dinPorts = 0;
	int usbDeviceJacks = 0;
	int usbHostJacks = 0;
	int ethernetJacks = 0;
	int usbMidiDevicePorts = 0;
	int usbMidiHostPorts = 0;
	int rtpMidiSessions = 0;
	int rtpMidiConnections = 0;
	int maximumSupportedUsbHostPorts = 0;

	bool enableRoutingBetweenHostJacks = false;
	bool enableRunningStatusOnDin = false;
};

#endif// RETSETMIDIINFO_H
