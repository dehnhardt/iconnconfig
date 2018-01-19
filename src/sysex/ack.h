#ifndef ACK_H
#define ACK_H

#include "sysexmessage.h"

class Ack : public SysExMessage {
public:
	Ack(Command m_Command, BYTE_VECTOR *message, Device *m_pDevice);
	void checkResult();
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	void parseAnswerData();

private:
	unsigned char m_cResult;
};

#endif// ACK_H
