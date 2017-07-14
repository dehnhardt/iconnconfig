#include "ack.h"

Ack::Ack(Command cmd, std::vector<unsigned char> *message, Device *device)
	: SysExMessage(cmd, message, device) {}

void Ack::checkResult() {
	switch (this->result) {
	case COMMAND_ACCEPTED:
#ifdef __MIO_DEBUG__
		std::cout << "Command accepted" << std::endl;
#endif
		return;
	case UNKNOWN_COMMAND:
		std::cerr << "Command not known by device" << std::endl;
		break;
	case MALFORMED_MESSAGE:
		std::cerr << "Message format has errors" << std::endl;
		break;
	case COMMAND_FAILED:
		std::cerr << "Message failed for unknown reason" << std::endl;
		break;
	default:
		std::cerr << "Unknown error code" << std::endl;
		break;
	}
}

void Ack::parseAnswerData() {
	if (data->size() == 3) {
		this->result = data->at(2);
	}
	checkResult();
}
