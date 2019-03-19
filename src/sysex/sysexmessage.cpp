#include "sysexmessage.h"
#include "ack.h"
#include "communicationexception.h"
#include "protocolexception.h"

#include <algorithm>
#include <iostream>
#include <unistd.h>

/*
 * Structure and naming od sysex data
 *
 * SYSEX_START 1Byte
 * MANUFACTURER_CODE 3Byte--| Header
 * MESSAGE_CLASS 1Byte ------
 *
 * PRODUCT_ID 2Byte --------| DeviceId -
 * SERIAL_NUMBER 5 Byte -----          |
 *                                     |
 * TRANSACTION_ID 2Byte ----|          | Body (Relevant for Checksum)
 * COMMAND 2Byte            |          |
 * DATA_LENGTH 2Byte        |          |
 * COMMAND_DATA xByte -------          |
 *                          -----------|
 * X_SUM 1yte
 *
 * SYSEX_END 1Byte
 *
 * ST| Man.Id |MC| PID | Serial No.   | TID | CID | LEN  | Data|CS|SE
 * 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17   ...  .. ..
 * F0 00 01 73 7E xx xx xx xx xx xx xx xx xx xx xx xx xx  xxxx  xx F7
 */

SysExMessage::SysExMessage(Command cmd, CommandFlags flags, Device *device)
	: m_Command(cmd), m_iCmdflags(flags), m_pDevice(device) {
	if (this->m_pDevice != nullptr)
		this->m_pDeviceHeader = this->m_pDevice->getDeviceHeader();
	else
		m_pDeviceHeader = new BYTE_VECTOR();
	m_pCommandData = new BYTE_VECTOR();
	m_pCommandData->push_back(flags);
	m_pCommandData->push_back(static_cast<unsigned char>(cmd));
	m_AcceptedAnswers = commandAcceptedAnswers[cmd];
}

SysExMessage::SysExMessage(Command cmd, std::vector<unsigned char> *message,
						   Device *device)
	: m_Command(cmd), m_pDevice(device) {
	if (this->m_pDevice != nullptr)
		this->m_pDeviceHeader = this->m_pDevice->getDeviceHeader();
	else
		m_pDeviceHeader = new BYTE_VECTOR();
	m_iCmdflags = (*message)[14];
	m_pCommandData = new BYTE_VECTOR();
	m_pCommandData->push_back(m_iCmdflags);
	m_pCommandData->push_back(static_cast<unsigned char>(cmd));
	m_AcceptedAnswers = commandAcceptedAnswers[cmd];
	extractData(message);
}

SysExMessage::~SysExMessage() {
	delete m_pCommandData;
	delete m_pDeviceHeader;
}

void SysExMessage::extractData(std::vector<unsigned char> *message) {
	m_iDataLength = static_cast<unsigned int>(MIDI::byteJoin7bit(
		new BYTE_VECTOR(message->begin() + Device::DATA_LENGTH_OFFSET,
						message->begin() + Device::DATA_LENGTH_OFFSET +
							Device::DATA_LENGTH_LENGTH)));
	m_pData =
		new BYTE_VECTOR(message->begin() + Device::DATA_OFFSET,
						message->begin() + Device::DATA_OFFSET + m_iDataLength);
}

Device *SysExMessage::getDevice() const { return m_pDevice; }

BYTE_VECTOR *SysExMessage::getMIDISysExMessage() {
	BYTE_VECTOR *body = new BYTE_VECTOR();
	BYTE_VECTOR *message = new BYTE_VECTOR();
	BYTE_VECTOR *manufacturerHeader = Device::getManufacturerHeader();

	BYTE_VECTOR *md = m_pGetMessageData();
	unsigned long mdSize = md->size();
	BYTE_VECTOR *bodyLength = MIDI::byteSplit7bit(mdSize, 2);
	BYTE_VECTOR *transactionId = getTransactionId();

	body->reserve(m_pDeviceHeader->size() + 6 + mdSize);
	body->insert(body->end(), m_pDeviceHeader->begin(), m_pDeviceHeader->end());
	body->insert(body->end(), transactionId->begin(), transactionId->end());
	body->insert(body->end(), getCommandData()->begin(),
				 getCommandData()->end());
	body->insert(body->end(), bodyLength->begin(), bodyLength->end());
	if (mdSize > 0)
		body->insert(body->end(), md->begin(), md->end());
	unsigned char cs = MIDI::RolandChecksum(body);

	message->reserve(manufacturerHeader->size() + m_pDeviceHeader->size() +
					 mdSize + 4);
	message->push_back(SYSEX_START);
	message->insert(message->end(), manufacturerHeader->begin(),
					manufacturerHeader->end());
	message->push_back(Device::MESSAGE_CLASS);
	message->insert(message->end(), body->begin(), body->end());
	message->push_back(cs);
	message->push_back(SYSEX_END);
	return message;
}

Command SysExMessage::parseAnswer(BYTE_VECTOR *answer) {
	if (!answer || answer->size() < 20)
		throw new ProtocolException(ProtocolException::MESSAGE_TO_SHORT);
	std::cout << "Answer: " << std::dec << answer->size() << std::endl;
	BYTE_VECTOR *commandBytes =
		new BYTE_VECTOR(answer->begin() + 14, answer->begin() + 16);
	long cb = MIDI::byteJoin7bit(commandBytes);
	int command = cb & 1023;
	try {
		checkAnswerValid(command);
		if (debug)
			std::cout << "Answer (command: " << std::hex << command
					  << ") accepted " << std::endl;
		extractData(answer);
		return static_cast<Command>(command);
	} catch (...) {
		throw;
	}
}

bool SysExMessage::checkAnswerValid(long answerCommandId) {
	if (std::find(m_AcceptedAnswers.begin(), m_AcceptedAnswers.end(),
				  answerCommandId) == m_AcceptedAnswers.end())
		throw ProtocolException(ProtocolException::WRONG_ANSWER);
	return true;
}

void SysExMessage::createAnswer(Command cmd,
								std::vector<unsigned char> *message,
								Device *device) {
	if (cmd == ACK) {
		m_pAnswer = new Ack(cmd, message, device);
		m_pAnswer->parseAnswerData();
	}
}

void SysExMessage::createAck(std::vector<unsigned char> *message,
							 Device *device) {
	m_pAnswer = new Ack(ACK, message, device);
	m_pAnswer->parseAnswerData();
}

void SysExMessage::readSettings() {}

void SysExMessage::storeSettings() {
	getSettingsId();
	getSettingsIndex();
	getStorableValue();
}

unsigned char SysExMessage::getCmdflags() const { return m_iCmdflags; }

void SysExMessage::setCmdflags(unsigned char value) { m_iCmdflags = value; }

void SysExMessage::printMessage() {
	std::cout << "Command Number: " << getCommandNumber() << "\n";
	std::cout << "Direction: " << (isWriteCommand() ? "write" : "read") << "\n";
}

int SysExMessage::execute() {
	if (m_pDevice == nullptr)
		return -1;
	BYTE_VECTOR *message = getMIDISysExMessage();
	if (debug) {
		printMessage();
		MIDI::printMessage(message);
	}
	m_pDevice->sentSysex(message);
	try {
		BYTE_VECTOR *answerMessage = nullptr;
		answerMessage = m_pDevice->retrieveSysex();
		if (answerMessage != nullptr) {
			Command cmd = parseAnswer(answerMessage);
			if (cmd == CMD_ERROR)
				throw(new ProtocolException(ProtocolException::UNKNOWN));
			// return -3;
			if (debug) {
				std::cout << std::hex << "Command number "
						  << (MIDI::byteJoin7bit(answerMessage, 14, 2) & 1023)
						  << "\n";
				std::cout << "c: ";
				MIDI::printMessage(answerMessage);
			}
			if (cmd != this->m_Command) {
				if (cmd == ACK)
					this->createAck(answerMessage, m_pDevice);
				else
					createAnswer(cmd, answerMessage, m_pDevice);
			}
			return 0;
		}
	} catch (...) {
		throw;
	}

	return -2;
}

void SysExMessage::setDebug(bool debug) { this->debug = debug; }

bool SysExMessage::getDebug() { return this->debug; }

void SysExMessage::printRawData() { MIDI::printMessage(m_pData); }

unsigned int SysExMessage::getCommandNumber() {
	long commandBytes = MIDI::byteJoin7bit(m_pCommandData);
	return static_cast<unsigned int>(commandBytes & 1023);
}

bool SysExMessage::isWriteCommand() {
	long commandBytes = MIDI::byteJoin7bit(m_pCommandData);
	return (commandBytes & 80192) == 1;
}

SysExMessage *SysExMessage::getAnswer() { return m_pAnswer; }

SysExMessage *SysExMessage::query() {
	try {
		execute();
	} catch (...) {
		throw;
	}

	return m_pAnswer;
}

std::string SysExMessage::getDataAsString() {
	std::string string2(m_pData->begin(), m_pData->end());
	return string2;
}

long SysExMessage::getDataAsLong() {
	long result = -1;
	if (m_pData->size() < 11)
		result = MIDI::byteJoin7bit(m_pData);
	return result;
}

CommandAcceptedAnswers SysExMessage::commandAcceptedAnswers = {
	{GET_DEVICE, AcceptedAnswers{RET_DEVICE}},
	{GET_COMMAND_LIST, AcceptedAnswers{RET_COMMAND_LIST}},
	{GET_INFO_LIST, AcceptedAnswers{RET_INFO_LIST}},
	{GET_INFO, AcceptedAnswers{RET_SET_INFO}},
	{RET_SET_INFO, AcceptedAnswers{ACK}},
	{GET_RESET_LIST, AcceptedAnswers{RET_RESET_LIST, ACK}},
	{GET_SAVE_RESTORE_LIST, AcceptedAnswers{RET_SAVE_RESTORE_LIST, ACK}},
	{GET_ETHERNET_PORT_INFO, AcceptedAnswers{RET_SET_ETHERNET_PORT_INFO, ACK}},
	{RET_SET_ETHERNET_PORT_INFO, AcceptedAnswers{ACK}},
	{RESET, AcceptedAnswers{ACK}},
	{SAVE_RESTORE, AcceptedAnswers{ACK}},
	{GET_GIZMO_COUNT, AcceptedAnswers{RET_GIZMO_COUNT, ACK}},
	{GET_GIZMO_INFO, AcceptedAnswers{RET_GIZMO_INFO, ACK}},
	{GET_MIDI_INFO, AcceptedAnswers{RET_SET_MIDI_INFO, ACK}},
	{GET_MIDI_PORT_INFO, AcceptedAnswers{RET_SET_MIDI_PORT_INFO, ACK}},
	{RET_SET_MIDI_PORT_INFO, AcceptedAnswers{ACK}},
	{GET_MIDI_PORT_FILTER, AcceptedAnswers{RET_SET_MIDI_PORT_FILTER, ACK}},
	{RET_SET_MIDI_PORT_FILTER, AcceptedAnswers{ACK}},
	{GET_MIDI_PORT_REMAP, AcceptedAnswers{RET_SET_MIDI_PORT_REMAP, ACK}},
	{RET_SET_MIDI_PORT_REMAP, AcceptedAnswers{ACK}},
	{GET_MIDI_PORT_ROUTE, AcceptedAnswers{RET_SET_MIDI_PORT_ROUTE, ACK}},
	{RET_SET_MIDI_PORT_ROUTE, AcceptedAnswers{ACK}},
	{GET_AUDIO_GLOBAL_PARM, AcceptedAnswers{RET_SET_AUDIO_GLOBAL_PARM}},
	{RET_SET_AUDIO_GLOBAL_PARM, AcceptedAnswers{ACK}},
	{GET_AUDIO_PORT_PARM, AcceptedAnswers{RET_SET_AUDIO_PORT_PARM, ACK}},
	{RET_SET_AUDIO_PORT_PARM, AcceptedAnswers{ACK}},
	{GET_AUDIO_DEVICE_PARM, AcceptedAnswers{RET_SET_AUDIO_DEVICE_PARM, ACK}},
	{GET_AUDIO_CONTROL_PARM, AcceptedAnswers{RET_SET_AUDIO_CONTROL_PARM, ACK}},
	{GET_AUDIO_CONTROL_DETAIL,
	 AcceptedAnswers{RET_SET_AUDIO_CONTROL_DETAIL, ACK}},
	{GET_AUDIO_CONTROL_DETAIL_VALUE,
	 AcceptedAnswers{RET_SET_AUDIO_CONTROL_DETAIL_VALUE, ACK}},
	{RET_SET_AUDIO_CONTROL_DETAIL_VALUE, AcceptedAnswers{ACK}}};
