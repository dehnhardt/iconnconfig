#ifndef PROTOCOLEXCEPTION_H
#define PROTOCOLEXCEPTION_H

#include "../device.h"

#include <sstream>
#include <stdexcept>
#include <string>

class ProtocolException : public std::runtime_error {
public:
	enum ProtocolErrorCode {
		UNKNOWN = 0,
		ACK_UNKNOWN_COMMAD,
		ACK_MALFORMED_MESSAGE,
		ACK_COMMAND_FAILED,
		MESSAGE_TO_SHORT,
		WRONG_VERSION,
		WRONG_ANSWER,
		WRONG_HEADER
	};

	ProtocolException(ProtocolErrorCode code = UNKNOWN, Device *device = 0)
		: std::runtime_error("CommunicationException"), m_ErrorCode(code),
		  m_pDevice(device) {}

	virtual ~ProtocolException();

	// getter
	ProtocolErrorCode getErrorCode() { return m_ErrorCode; }
	std::string getErrorMessage() {
		std::stringstream e;
		e << "Protocol error. Reason: ";
		switch (m_ErrorCode) {
		case UNKNOWN:
			e << " unknown";
			break;
		case ACK_UNKNOWN_COMMAD:
			e << " the command is unknown";
			break;
		case ACK_MALFORMED_MESSAGE:
			e << " the message is malformed";
			break;
		case ACK_COMMAND_FAILED:
			e << " the command failed for an unknown reason";
			break;
		case MESSAGE_TO_SHORT:
			e << " the the length of the message is shorten than 20 chars";
			break;
		case WRONG_VERSION:
			e << " the protocol version does not match";
			break;
		case WRONG_ANSWER:
			e << " the answer is not allowed to this command";
			break;
		case WRONG_HEADER:
			e << " the answer has an unexpected header";
			break;
		default:
			break;
		}
		e << std::endl;
		if (m_pDevice) {
			e << "Query: "
			  << MIDI::printMessageToHexString(m_pDevice->getLastSendMessage())
			  << std::endl;

			e << "Answer: "
			  << MIDI::printMessageToHexString(m_pDevice->getLastRetrieveMessage())
			  << std::endl;
		}
		return e.str();
	}

private:
	ProtocolErrorCode m_ErrorCode = UNKNOWN;
	Device *m_pDevice;
};

#endif// PROTOCOLEXCEPTION_H
