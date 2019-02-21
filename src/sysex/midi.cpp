#include "midi.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>

MIDI::MIDI() {}

RtMidiIn *MIDI::createMidiIn(const std::string clientName) {
	// RtMidiIn constructor
	RtMidiIn *midiin = nullptr;
	try {
		midiin = new RtMidiIn(RtMidi::LINUX_ALSA, clientName);
	} catch (RtMidiError &error) {
		// TODO Handle the exception here
		error.printMessage();
	}
	midiin->ignoreTypes(false, true, true);
	return midiin;
}

RtMidiOut *
MIDI::createMidiOut(const std::string clientName) // RtMidiOut constructor
{
	RtMidiOut *midiout = nullptr;
	try {
		midiout = new RtMidiOut(RtMidi::LINUX_ALSA, clientName);
	} catch (RtMidiError &error) {
		// TODO Handle the exception here
		error.printMessage();
	}
	return midiout;
}

unsigned char MIDI::RolandChecksum(BYTE_VECTOR *message) {
	unsigned long nBytes = message->size();
	unsigned long sum = 0;
	for (unsigned int i = 0; i < nBytes; i++) {
		sum += static_cast<unsigned long>(message->at(i));
		if (sum > 127)
			sum -= 128;
	}
	return static_cast<unsigned char>(sum == 0 ? sum : 128 - sum);
}

BYTE_VECTOR *MIDI::byteSplit7bit(unsigned long val) {
	return MIDI::byteSplit7bit(val, 0);
}

BYTE_VECTOR *MIDI::byteSplit7bit(unsigned long val, unsigned long size) {
	BYTE_VECTOR *bytes = new BYTE_VECTOR();
	bytes->reserve(size);
	while (val > 0) {
		unsigned char c = val & 0x7f;
		val >>= 7;
		bytes->push_back(c);
	}
	if (size > 0) {
		unsigned long nLength = bytes->size();
		if (nLength < size) {
			for (unsigned long i = nLength; i < size; i++)
				bytes->push_back(0x00);
		}
	}
	std::reverse(bytes->begin(), bytes->end());
	return bytes;
}

BYTE_VECTOR *MIDI::byteSplit8bit(long val) {
	return MIDI::byteSplit8bit(val, 0);
}

BYTE_VECTOR *MIDI::byteSplit8bit(long val, unsigned long size) {
	BYTE_VECTOR *bytes = new BYTE_VECTOR();
	bytes->reserve(size);
	while (val > 0) {
		unsigned char c = val & 0xff;
		val >>= 8;
		bytes->push_back(c);
	}
	if (size > 0) {
		unsigned long nLength = bytes->size();
		if (nLength < size) {
			for (unsigned long i = nLength; i < size; i++)
				bytes->push_back(0x00);
		}
	}
	std::reverse(bytes->begin(), bytes->end());
	return bytes;
}

long MIDI::byteJoin7bit(BYTE_VECTOR *message) {
	return byteJoin7bit(message, 0, message->size());
}

long MIDI::byteJoin7bit(BYTE_VECTOR *message, unsigned long start,
						unsigned long length) {
	unsigned long cnt;
	long current = 0;

	if (start + length > message->size())
		return -1;

	for (cnt = start; cnt < start + length; cnt++) {
		current <<= 7;
		current += message->at(cnt);
	}
	return current;
}

long MIDI::byteJoin8bit(BYTE_VECTOR *message) {
	return byteJoin8bit(message, 0, message->size());
}

long MIDI::byteJoin8bit(BYTE_VECTOR *message, unsigned long start,
						unsigned long length) {
	unsigned long cnt;
	long current = 0;

	if (start + length > message->size())
		return -1;

	for (cnt = start; cnt < start + length; cnt++) {
		current <<= 8;
		current += message->at(cnt);
	}
	return current;
}

std::string MIDI::decodeIp(BYTE_VECTOR *data, unsigned long offset) {

	long address = MIDI::byteJoin7bit(data, offset, 5);
	int a1 = address & 255;
	address >>= 8;
	int a2 = address & 255;
	address >>= 8;
	int a3 = address & 255;
	address >>= 8;
	int a4 = address & 255;
	std::stringstream result;
	result << std::dec << a4 << "." << a3 << "." << a2 << "." << a1;
	std::string ad = result.str();
	return ad;
}

BYTE_VECTOR *MIDI::encodeIpAddress(std::string ipAddress) {
	BYTE_VECTOR *result = nullptr;

	unsigned long lAddress = 0L;
	std::istringstream ss(ipAddress);
	std::string token;
	std::vector<std::string> ipParts;

	while (std::getline(ss, token, '.'))
		ipParts.push_back(std::string(token));
	for (std::vector<std::string>::iterator it = ipParts.begin();
		 it != ipParts.end(); ++it) {
		if (it != ipParts.begin())
			lAddress <<= 8;
		token = (*it);
		lAddress += static_cast<unsigned long>(std::stol(token));
	}
	result = MIDI::byteSplit7bit(lAddress, 5);
	return result;
}

void MIDI::printMessage(BYTE_VECTOR *message) {
	if (!message)
		return;
	unsigned long nMessageSize = message->size();
	std::cout << std::hex;
	for (unsigned int i = 0; i < nMessageSize; i++)
		std::cout << std::setw(2) << std::setfill('0')
				  << static_cast<int>(message->at(i)) << " ";
	std::cout << "\n" << std::flush;
}

std::string MIDI::printMessageToHexString(BYTE_VECTOR *message) {
	unsigned long nMessageSize = message->size();
	std::stringstream ss;
	if (message) {
		ss << std::hex;
		for (unsigned int i = 0; i < nMessageSize; i++) {
			ss << std::setw(2) << std::setfill('0')
			   << static_cast<int>(message->at(i)) << " ";
		}
	}
	std::cout << std::flush;
	return ss.str();
}
