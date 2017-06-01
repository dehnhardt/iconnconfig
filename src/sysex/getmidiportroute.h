#ifndef GETMIDIPORTROUTE_H
#define GETMIDIPORTROUTE_H

#include "sysexmessage.h"

class GetMidiPortRoute : public SysExMessage {
public:
	GetMidiPortRoute(Device *device);
	void createAnswer(SysExMessage::Command cmd,
										std::vector<unsigned char> *message, Device *device);

public:
	// setter
	void setPortNumer(int portNumber) { this->portNumber = portNumber; }

protected:
	std::vector<unsigned char> *getMessageData();

private:
	int portNumber;
};

#endif // GETMIDIPORTROUTE_H
