#ifndef COMMUNICATIONEXCEPTION_H
#define COMMUNICATIONEXCEPTION_H

#include "../RtMidi.h"
#include "../device.h"

#include <sstream>
#include <stdexcept>
#include <string>

class CommunicationException : public std::runtime_error {
public:
	enum CommunicationErrorCode {
		UNKNOWN = 0,
		ANSWER_TIMEOOUT = 1,
		NO_DEVICES_FOUND,
		INVALID_DEVICE,
		INVALID_PARAMETER,
		INVALID_USE,
	};

	CommunicationException(CommunicationErrorCode code = UNKNOWN,
						   Device *device = 0)
		: std::runtime_error("CommunicationException"), code(code),
		  device(device) {}

	CommunicationException(RtMidiError error)
		: std::runtime_error("CommunicationException") {
		switch (error.getType()) {
		case RtMidiError::INVALID_PARAMETER:
			code = INVALID_PARAMETER;
			break;
		case RtMidiError::NO_DEVICES_FOUND:
			code = NO_DEVICES_FOUND;
			break;
		case RtMidiError::INVALID_DEVICE:
			code = INVALID_DEVICE;
			break;
		case RtMidiError::INVALID_USE:
			code = INVALID_USE;
			break;
		default:
			break;
		}
		errorString = error.getMessage();
	}

	virtual ~CommunicationException();
	// getter
	CommunicationErrorCode getErrorCode() { return code; }
	std::string getErrorMessage() {
		if (errorString.size() > 0)
			return errorString;
		std::stringstream e;
		e << "Error when communicationg with device. Reason: ";
		switch (code) {
		case UNKNOWN:
			e << " unknown. ";
			break;
		case ANSWER_TIMEOOUT:
			e << " timeout while waiting for the answer. ";
			if (device)
				e << std::endl
				  << "Query: " << MIDI::printMessageToHexString(
									  device->getLastSendMessage());
			break;
		case INVALID_PARAMETER:
			e << " the port number given is invalid. ";
			break;
		case NO_DEVICES_FOUND:
			e << " no devices are found. ";
			break;
		case INVALID_USE:
			e << " invalid use of the RtMidiApi. ";
			break;
		default:
			e << " unspecified error";
			break;
		}
		e << std::endl;
		return e.str();
	}

private:
	CommunicationErrorCode code = UNKNOWN;
	Device *device = 0;
	std::string errorString;
};

#endif// COMMUNICATIONEXCEPTION_H
