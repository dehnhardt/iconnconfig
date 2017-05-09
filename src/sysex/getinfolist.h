#ifndef GETINFOLIST_H
#define GETINFOLIST_H

#include "implementedinfos.h"
#include "sysexmessage.h"

class GetInfoList : public SysExMessage {
public:
	GetInfoList(Device *device);
	GetInfoList(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

private:
	void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
										Device *device) {
		answer = new ImplementedInfos(cmd, message, device);
		answer->parseAnswerData();
	}
};

#endif // GETINFOLIST_H
