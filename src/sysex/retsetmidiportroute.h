#ifndef RETSETMIDIPORTROUTE_H
#define RETSETMIDIPORTROUTE_H

#include "sysexmessage.h"

class RetSetMidiPortRoute : public SysExMessage {
public:
	RetSetMidiPortRoute(Device *device);

	RetSetMidiPortRoute(Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}

	void parseAnswerData();
	bool isPortRouted(int portNumber);

private:
	long portId;
	BYTE_VECTOR *portRoutings = 0;
};

#endif // RETSETMIDIPORTROUTE_H
