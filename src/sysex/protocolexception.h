#ifndef PROTOCOLEXCEPTION_H
#define PROTOCOLEXCEPTION_H

#include <stdexcept>
#include <string>
#include <strstream>

class ProtocolException : public std::runtime_error {
public:
	enum ProtocolErrorCode {
		UNKNOWN = 0,
		ACK_UNKNOWN_COMMAD,
		ACK_MALFORMED_MESSAGE,
		ACK_COMMAND_FAILED,
		WRONG_ANSWER
	};

	ProtocolException(ProtocolErrorCode code = UNKNOWN)
			: std::runtime_error("CommunicationException"), code(code) {}

	// getter
	ProtocolErrorCode getErrorCode() { return code; }
	std::string getErrorMessage() {
		std::strstream e;
		e << "Error when communicationg with device. Reason: ";
		switch (code) {
		case UNKNOWN:
			e << " unknown";
			break;
		case ANSWER_TIMEOOUT:
			e << " the answer had a timeout";
		default:
			break;
		}
		e << std::endl;
		return e.str();
	}

private:
	ProtocolErrorCode code = UNKNOWN;
};

#endif // PROTOCOLEXCEPTION_H
