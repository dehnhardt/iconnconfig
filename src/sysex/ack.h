#ifndef ACK_H
#define ACK_H

#include "sysexmessage.h"

class Ack : public SysExMessage {
public:
	Ack(Command cmd, BYTE_VECTOR *message, Device *device);
	void checkResult();
	int getSettingsId() { return cmd; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }

protected:
	void parseAnswerData();

private:
	unsigned char result;
};

#endif// ACK_H
