#ifndef COMMUNICATIONEXCEPTION_H
#define COMMUNICATIONEXCEPTION_H

#include <stdexcept>
#include <string>
#include <strstream>

class CommunicationException : public std::runtime_error {
public:
	enum CommunicationErrorCode { UNKNOWN = 0, ANSWER_TIMEOOUT = 1 };

	CommunicationException(CommunicationErrorCode code = UNKNOWN)
			: std::runtime_error("CommunicationException"), code(code) {}

	// getter
	CommunicationErrorCode getErrorCode() { return code; }
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
	CommunicationErrorCode code = UNKNOWN;
};

#endif // COMMUNICATIONEXCEPTION_H
