#include "retsetmidiportremap.h"
#include <cmath>

RetSetMidiPortRemap::RetSetMidiPortRemap(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_REMAP, SysExMessage::QUERY,
				   device) {}

RetSetMidiPortRemap::~RetSetMidiPortRemap() {
	delete *m_pMidiPortRemap->midiControllerRemap;
	for (int i = 0; i < MIDI_CHANNELS; i++) {
		delete m_pMidiPortRemap->midiChannelMessagesRemap[i];
	}
	delete m_pMidiPortRemap;
}

void RetSetMidiPortRemap::parseAnswerData() {
	if (!m_pMidiPortRemap)
		m_pMidiPortRemap = new MIDIPortRemap();
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = MIDI::byteJoin7bit(m_pData, 1, 2);
	m_pMidiPortRemap->portRemapDirection =
		m_pData->at(3) == 1 ? PortDirection::INPUT : PortDirection::OUTPUT;
	m_pMidiPortRemap->numberOfControllerRemaps = m_pData->at(4);
	m_pMidiPortRemap->midiControllerRemap =
		new MIDIControllerRemap *[m_pMidiPortRemap->numberOfControllerRemaps];
	for (unsigned int i = 0; i < 16; i++) {
		parseMidiChannelMessagesRemap(i);
	}
	for (unsigned int i = 0; i < m_pMidiPortRemap->numberOfControllerRemaps;
		 i++) {
		parseMidiControllerRemap(i);
	}
}

void RetSetMidiPortRemap::parseMidiChannelMessagesRemap(
	unsigned int midiChannel) {
	unsigned int remap = static_cast<unsigned int>(
		MIDI::byteJoin8bit(m_pData, 5 + 2 * midiChannel, 2));
	MIDIChannelMessagesRemap *midiChannelMessagesRemap =
		new MIDIChannelMessagesRemap();
	midiChannelMessagesRemap->remapMidiPitchBendEvents = remap & 8192;
	midiChannelMessagesRemap->remapMidiChannelPressureEvents = remap & 4096;
	midiChannelMessagesRemap->remapMidiProgrammChangeEvents = remap & 2048;
	midiChannelMessagesRemap->remapMidiControlChangeEvents = remap & 1024;
	midiChannelMessagesRemap->remapMidiPolyKeyPressureEvents = remap & 512;
	midiChannelMessagesRemap->remapMidiNoteOnOffEvents = remap & 256;
	midiChannelMessagesRemap->remapChannel = (remap & 15) + 1;
	m_pMidiPortRemap->midiChannelMessagesRemap[midiChannel] =
		midiChannelMessagesRemap;
}

void RetSetMidiPortRemap::parseMidiControllerRemap(
	unsigned int contollerRemapNumber) {
	unsigned int datalength = 6;
	unsigned int offset = 37 + (datalength * contollerRemapNumber);
	if (offset + datalength <= m_pData->size()) {
		MIDIControllerRemap *midiControllerRemap = new MIDIControllerRemap();
		midiControllerRemap->midiContollerSourceNumber =
			m_pData->at(offset + 4);
		midiControllerRemap->midiContollerDestinationNumber =
			m_pData->at(offset + 5);
		long midiChannelBitmap = MIDI::byteJoin8bit(m_pData, offset, 4);
		if (debug) {
			std::cout << "Midi Controller Remap" << contollerRemapNumber
					  << ": From "
					  << midiControllerRemap->midiContollerSourceNumber
					  << " To "
					  << midiControllerRemap->midiContollerDestinationNumber
					  << " - ChanelBitmap: " << midiChannelBitmap << std::endl;
		}
		for (int i = 0; i < 16; i++) {
			int midiChannelMask = i;
			if (i > 15)
				midiChannelMask += 16;
			else if (i > 11)
				midiChannelMask += 12;
			else if (i > 7)
				midiChannelMask += 8;
			else if (i > 3)
				midiChannelMask += 4;
			long bitmask = static_cast<long>(
				pow(2.0, static_cast<double>(midiChannelMask)));
			long val = midiChannelBitmap & bitmask;
			midiControllerRemap->channel[i] = val > 0;
		}
		m_pMidiPortRemap->midiControllerRemap[contollerRemapNumber] =
			midiControllerRemap;
	}
}

std::vector<unsigned char> *
RetSetMidiPortRemap::createMidiChannelMessagesRemapData() {
	MIDIChannelMessagesRemap **remaps =
		m_pMidiPortRemap->midiChannelMessagesRemap;
	BYTE_VECTOR *data = new BYTE_VECTOR();
	for (int i = 0; i < MIDI_CHANNELS; i++) {
		MIDIChannelMessagesRemap *midiChannelMessagesRemap = remaps[i];
		long remap = 0;
		if (midiChannelMessagesRemap->remapMidiPitchBendEvents)
			remap += 8192;
		if (midiChannelMessagesRemap->remapMidiChannelPressureEvents)
			remap += 4096;
		if (midiChannelMessagesRemap->remapMidiProgrammChangeEvents)
			remap += 2048;
		if (midiChannelMessagesRemap->remapMidiControlChangeEvents)
			remap += 1024;
		if (midiChannelMessagesRemap->remapMidiPolyKeyPressureEvents)
			remap += 512;
		if (midiChannelMessagesRemap->remapMidiNoteOnOffEvents)
			remap += 256;
		if (midiChannelMessagesRemap->remapChannel > 0)
			remap += midiChannelMessagesRemap->remapChannel - 1;
		BYTE_VECTOR *v = MIDI::byteSplit8bit(remap, 2);
		data->insert(data->end(), v->begin(), v->end());
		delete v;
	}
	return data;
}

std::vector<unsigned char> *
RetSetMidiPortRemap::createMidiControllerRemapData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	MIDIControllerRemap **remaps = m_pMidiPortRemap->midiControllerRemap;
	unsigned int numberOfControllers = sizeof(remaps);
	for (unsigned int i = 0; i < numberOfControllers; i++) {
		MIDIControllerRemap *remap = remaps[i];
		long iChannelMatrix = 0;
		for (unsigned int midiChannel = 0; midiChannel < MIDI_CHANNELS;
			 midiChannel++) {
			if (remap->channel[midiChannel]) {
				if (midiChannel < 4)
					iChannelMatrix += pow(2, midiChannel);
				else if (midiChannel < 8)
					iChannelMatrix += pow(2, (midiChannel + 4));
				else if (midiChannel < 12)
					iChannelMatrix += pow(2, (midiChannel + 8));
				else if (midiChannel < 16)
					iChannelMatrix += pow(2, (midiChannel + 12));
			}
		}
		BYTE_VECTOR *channelMatrix = MIDI::byteSplit8bit(iChannelMatrix, 4);
		data->insert(data->end(), channelMatrix->begin(), channelMatrix->end());
		data->push_back(
			static_cast<unsigned char>(remap->midiContollerSourceNumber));
		data->push_back(
			static_cast<unsigned char>(remap->midiContollerDestinationNumber));
		delete channelMatrix;
	}
	return data;
}

std::vector<unsigned char> *RetSetMidiPortRemap::m_pGetMessageData() {
	// Write Flag
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *portIdV =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	BYTE_VECTOR *midiChannelMessagesRemapData =
		createMidiChannelMessagesRemapData();
	BYTE_VECTOR *midiControllerMessagesRemapData =
		createMidiControllerRemapData();
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	// Command Version Number
	messageData->push_back(m_iCommandVersionNumber);
	// Portnumber
	messageData->insert(messageData->end(), portIdV->begin(), portIdV->end());
	// Remap Direction
	messageData->push_back(m_pMidiPortRemap->portRemapDirection);
	// Number of Controller Remaps
	messageData->push_back(static_cast<unsigned int>(
		sizeof(m_pMidiPortRemap->numberOfControllerRemaps)));
	// Midi Channel Messages Data
	messageData->insert(messageData->end(),
						midiChannelMessagesRemapData->begin(),
						midiChannelMessagesRemapData->end());
	// Midi Controller Messages Data
	messageData->insert(messageData->end(),
						midiControllerMessagesRemapData->begin(),
						midiControllerMessagesRemapData->end());
	delete portIdV;
	delete midiChannelMessagesRemapData;
	delete midiControllerMessagesRemapData;
	return messageData;
}
