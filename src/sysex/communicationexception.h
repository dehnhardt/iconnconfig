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
		: std::runtime_error("CommunicationException"), m_code(code),
		  m_pDevice(device) {}

	CommunicationException(RtMidiError error)
		: std::runtime_error("CommunicationException") {
		switch (error.getType()) {
		case RtMidiError::INVALID_PARAMETER:
			m_code = INVALID_PARAMETER;
			break;
		case RtMidiError::NO_DEVICES_FOUND:
			m_code = NO_DEVICES_FOUND;
			break;
		case RtMidiError::INVALID_DEVICE:
			m_code = INVALID_DEVICE;
			break;
		case RtMidiError::INVALID_USE:
			m_code = INVALID_USE;
			break;
		default:
			break;
		}
		m_sErrorString = error.getMessage();
	}

	virtual ~CommunicationException();
	// getter
	CommunicationErrorCode getErrorCode() { return m_code; }
	std::string getErrorMessage() {
		if (m_sErrorString.size() > 0)
			return m_sErrorString;
		std::stringstream e;
		e << "Error when communicationg with device. Reason: ";
		switch (m_code) {
		case UNKNOWN:
			e << " unknown. ";
			break;
		case ANSWER_TIMEOOUT:
			e << " timeout while waiting for the answer. ";
			if (m_pDevice)
				e << std::endl
				  << "Query: " << MIDI::printMessageToHexString(
									  m_pDevice->getLastSendMessage());
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
	CommunicationErrorCode m_code = UNKNOWN;
	Device *m_pDevice = 0;
	std::string m_sErrorString;
};

#endif// COMMUNICATIONEXCEPTION_H
