#ifndef GETMIDIPORTROUTE_H
#define GETMIDIPORTROUTE_H

#include "sysexmessage.h"

class GetMidiPortRoute : public SysExMessage {
public:
	GetMidiPortRoute(Device *device);
	void createAnswer(SysExMessage::Command cmd,
					  std::vector<unsigned char> *message, Device *device);
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

public:
	// setter
	void setPortNumer(int portNumber) { this->portNumber = portNumber; }

protected:
	std::vector<unsigned char> *getMessageData();

private:
	int portNumber;
};

#endif// GETMIDIPORTROUTE_H
