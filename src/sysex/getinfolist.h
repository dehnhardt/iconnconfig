#ifndef GETINFOLIST_H
#define GETINFOLIST_H

#include "retinfolist.h"
#include "sysexmessage.h"

class GetInfoList : public SysExMessage {
public:
	GetInfoList(Device *device);
	GetInfoList(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	virtual ~GetInfoList() {}
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		answer = new RetInfoList(cmd, message, device);
		answer->parseAnswerData();
	}
};

#endif// GETINFOLIST_H
