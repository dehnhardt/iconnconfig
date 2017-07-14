#include "retsetmidiportroute.h"

#include <cmath>

RetSetMidiPortRoute::RetSetMidiPortRoute(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_INFO, SysExMessage::QUERY,
				   device) {}

void RetSetMidiPortRoute::parseAnswerData() {
	portId = MIDI::byteJoin(data, 1, 2);
	portRoutings = new BYTE_VECTOR(data->begin() + 3, data->end());
}

bool RetSetMidiPortRoute::isPortRouted(int portNumber) {
	int byte = -1;
	int bit = -1;
	getPortByteAndBit(portNumber, byte, bit);
	if (byte <= numerOfExpectedBytes) {
		unsigned int byteVal = portRoutings->at(byte);
		int mask = pow(2, bit);
		return byteVal & mask;
	}
	return false;
}

void RetSetMidiPortRoute::setPortRouted(int portNumber, bool routed) {
	int byte = -1;
	int bit = -1;
	int val = 0;
	getPortByteAndBit(portNumber, byte, bit);
	if (byte != -1 && bit != -1)
		val = portRoutings->at(byte);
	int mask = pow(2, bit);
	if (routed) {
		val |= mask;
	} else {
		val &= ~mask;
	}
	(*portRoutings)[byte] = val;
}

int RetSetMidiPortRoute::getTotalNumberOfPorts() const {
	return totalNumberOfPorts;
}

void RetSetMidiPortRoute::setTotalNumberOfPorts(int value) {
	totalNumberOfPorts = value;
	numerOfExpectedBytes = getNumberOfExpectedBytes(totalNumberOfPorts);
}

std::vector<unsigned char> *RetSetMidiPortRoute::getMessageData() {
	this->command->at(0) = 0x40;
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	messageData->push_back(0x01);
	BYTE_VECTOR *portIdV = MIDI::byteSplit(portId, 2);
	messageData->insert(messageData->end(), portIdV->begin(), portIdV->end());
	messageData->insert(messageData->end(), portRoutings->begin(),
						portRoutings->end());
	return messageData;
}

void RetSetMidiPortRoute::getPortByteAndBit(int portNumber, int &byte,
											int &bit) {
	--portNumber;
	if (numerOfExpectedBytes != -1 &&
		(unsigned int)numerOfExpectedBytes >= portRoutings->size())
		byte = portNumber / 4;
	bit = portNumber - (byte * 4);
}

int RetSetMidiPortRoute::getNumberOfExpectedBytes(int totalPortNumber) {
	return (((totalPortNumber - 1) / 8) + 1) * 2;
}
