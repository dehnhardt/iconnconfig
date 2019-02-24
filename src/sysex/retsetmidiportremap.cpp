#include "retsetmidiportremap.h"
#include <cmath>

RetSetMidiPortRemap::RetSetMidiPortRemap(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_FILTER, SysExMessage::QUERY,
				   device) {}

RetSetMidiPortRemap::~RetSetMidiPortRemap() { delete m_pMidiPortRemap; }

void RetSetMidiPortRemap::parseAnswerData() {
    if (!m_pMidiPortRemap)
        m_pMidiPortRemap = new MIDIPortRemap();
	m_iCommandVersion = m_pData->at(0);
	m_iPortId = MIDI::byteJoin7bit(m_pData, 1, 2);
    m_pMidiPortRemap->portRemapDirection = m_pData->at(3) == 1
                                                 ? PortRemapDirection::INPUT
                                                 : PortRemapDirection::OUTPUT;
    m_pMidiPortRemap->numberOfControllerRemaps = m_pData->at(4);
    m_pMidiPortRemap->midiControllerRemap = new MIDIControllerRemap
        *[m_pMidiPortRemap->numberOfControllerRemaps];
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
	unsigned int filter = m_pData->at(7 + midiChannel);
    MIDIChannelMessagesRemap *midiChannelMessagesRemap =
        new MIDIChannelMessagesRemap();
    midiChannelMessagesRemap->remapMidiPitchBendEvents = filter & 32;
    midiChannelMessagesRemap->remapMidiChannelPressureEvents = filter & 16;
    midiChannelMessagesRemap->remapMidiProgrammChangeEvents = filter & 8;
    midiChannelMessagesRemap->remapMidiControlChangeEvents = filter & 4;
    midiChannelMessagesRemap->remapMidiPolyKeyPressureEvents = filter & 2;
    midiChannelMessagesRemap->remapMidiNoteOnOffEvents = filter & 1;
    m_pMidiPortRemap->midiChannelMessagesRemap[midiChannel] =
        midiChannelMessagesRemap;
}

void RetSetMidiPortRemap::parseMidiControllerRemap(
    unsigned int contollerRemapNumber) {
	unsigned int datalength = 5;
    unsigned int offset = 23 + (datalength * contollerRemapNumber);
	if (offset + datalength <= m_pData->size()) {
        MIDIControllerRemap *midiControllerFilter = new MIDIControllerRemap();
		unsigned int midiControllerNumber = m_pData->at(offset + 4);
		long midiChannelBitmap = MIDI::byteJoin8bit(m_pData, offset, 4);
		if (debug) {
            std::cout << "Midi Controller Filter" << contollerRemapNumber
					  << ": " << midiControllerNumber
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
			midiControllerFilter->channel[i] = val > 0;
		}
		midiControllerFilter->midiContollerNumber = midiControllerNumber;
        m_pMidiPortRemap->midiControllerRemap[contollerRemapNumber] =
			midiControllerFilter;
	}
}

std::vector<unsigned char> *
RetSetMidiPortRemap::createMidiChannelMessagesRemapData() {
    MIDIChannelMessagesRemap **remaps =
        m_pMidiPortRemap->midiChannelMessagesRemap;
	BYTE_VECTOR *data = new BYTE_VECTOR();
	for (int i = 0; i < MIDI_CHANNELS; i++) {
        MIDIChannelMessagesRemap *filter = remaps[i];
		unsigned char b = 0;
        if (filter->remapMidiPitchBendEvents)
			b += 32;
        if (filter->remapMidiChannelPressureEvents)
			b += 16;
        if (filter->remapMidiProgrammChangeEvents)
			b += 8;
        if (filter->remapMidiControlChangeEvents)
			b += 4;
        if (filter->remapMidiPolyKeyPressureEvents)
			b += 2;
        if (filter->remapMidiNoteOnOffEvents)
			b += 1;
		data->push_back(b);
	}
	return data;
}

std::vector<unsigned char> *
RetSetMidiPortRemap::createMidiControllerRemapData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
    MIDIControllerRemap **remaps = m_pMidiPortRemap->midiControllerRemap;
    unsigned int numberOfFilters = sizeof(remaps);
	for (unsigned int i = 0; i < numberOfFilters; i++) {
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
            static_cast<unsigned char>(remap->midiContollerNumber));
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
	// Filter Direction
    messageData->push_back(m_pMidiPortRemap->portRemapDirection);
	// Number of Controller Filters
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
	return messageData;
}
