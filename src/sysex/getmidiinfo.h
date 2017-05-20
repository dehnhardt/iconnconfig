#ifndef GETMIDIINFO_H
#define GETMIDIINFO_H

#include "retsetmidiinfo.h"
#include "sysexmessage.h"

class GetMidiInfo : public SysExMessage {
public:
	GetMidiInfo(Device *device);
	GetMidiInfo(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
			: SysExMessage(cmd, message, device) {}

private:
	void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
										Device *device) {
		answer = new RetSetMidiInfo(cmd, message, device);
		answer->parseAnswerData();
	}
};

#endif // GETMIDIINFO_H
