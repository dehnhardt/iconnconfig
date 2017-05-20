#ifndef RETSETMIDIINFO_H
#define RETSETMIDIINFO_H

#include "sysexmessage.h"

class RetSetMidiInfo : public SysExMessage {
public:
	RetSetMidiInfo(Device *device);

	RetSetMidiInfo(Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}

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

#endif // RETSETMIDIINFO_H
