#ifndef COMMUNICATIONEXCEPTION_H
#define COMMUNICATIONEXCEPTION_H

#include <sstream>
#include <stdexcept>
#include <string>

class CommunicationException : public std::runtime_error {
public:
	enum CommunicationErrorCode { UNKNOWN = 0, ANSWER_TIMEOOUT = 1 };

	CommunicationException(CommunicationErrorCode code = UNKNOWN)
		: std::runtime_error("CommunicationException"), code(code) {}
	virtual ~CommunicationException();
	// getter
	CommunicationErrorCode getErrorCode() { return code; }
	std::string getErrorMessage() {
		std::stringstream e;
		e << "Error when communicationg with device. Reason: ";
		switch (code) {
		case UNKNOWN:
			e << " unknown. ";
			break;
		case ANSWER_TIMEOOUT:
			e << " timeout whil waiting for the answer. ";
			break;
		default:
			break;
		}
		e << std::endl;
		return e.str();
	}

private:
	CommunicationErrorCode code = UNKNOWN;
};

#endif// COMMUNICATIONEXCEPTION_H
