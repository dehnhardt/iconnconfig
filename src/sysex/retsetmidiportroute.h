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
	void setPortRouted(int portNumber, bool routed);

	int getTotalNumberOfPorts() const;
	void setTotalNumberOfPorts(int value);
	BYTE_VECTOR *getMessageData();

private:
	long portId;
	int totalNumberOfPorts = -1;
	int numerOfExpectedBytes = -1;
	BYTE_VECTOR *portRoutings = 0;
	int getNumberOfExpectedBytes(int totalPortNumber);
	void getPortByteAndBit(int portNumber, int &byte, int &bit);
};

#endif // RETSETMIDIPORTROUTE_H
