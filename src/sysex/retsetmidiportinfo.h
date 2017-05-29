#ifndef RETSETMIDIPORTINFO_H
#define RETSETMIDIPORTINFO_H

#include "sysexmessage.h"

class RetSetMidiPortInfo : public SysExMessage {
public:
	RetSetMidiPortInfo(Device *device);

	RetSetMidiPortInfo(Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}

	enum MidiPortType { DIN, USB_DEVICE, USB_HOST, ETHERNET };
};

#endif // RETSETMIDIPORTINFO_H
