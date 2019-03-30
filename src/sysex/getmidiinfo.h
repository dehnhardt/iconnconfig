#ifndef GETMIDIINFO_H
#define GETMIDIINFO_H

#include "retsetmidiinfo.h"
#include "sysexmessage.h"

class GetMidiInfo : public SysExMessage {
public:
	GetMidiInfo(Device *m_pDevice);
	GetMidiInfo(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

private:
	void createAnswer(Command cmd, BYTE_VECTOR *message, Device *device) {
		m_pAnswer = std::make_shared<RetSetMidiInfo>(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
};

#endif// GETMIDIINFO_H
