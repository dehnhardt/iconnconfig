#ifndef GETMIDIPORTINFO_H
#define GETMIDIPORTINFO_H

#include "retsetmidiportinfo.h"
#include "sysexmessage.h"

class GetMidiPortInfo : public SysExMessage {
public:
	GetMidiPortInfo(Device *device);
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

#endif// GETMIDIPORTINFO_H
