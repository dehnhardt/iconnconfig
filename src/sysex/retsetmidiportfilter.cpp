#include "retsetmidiportfilter.h"
#include <cmath>

RetSetMidiPortFilter::RetSetMidiPortFilter(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_FILTER, SysExMessage::QUERY,
				   device) {}

RetSetMidiPortFilter::~RetSetMidiPortFilter() {

	delete m_pMidiPortFilter->midiSystemMessagesFilter;
	for (int i = 0; i < MIDI_CHANNELS; i++)
		delete m_pMidiPortFilter->midiChannelMessagesFilter[i];
	delete *m_pMidiPortFilter->midiControllerFilter;
	delete m_pMidiPortFilter;
}

void RetSetMidiPortFilter::parseAnswerData() {
	if (!m_pMidiPortFilter)
		m_pMidiPortFilter = new MIDIPortFilter();
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = MIDI::byteJoin7bit(m_pData, 1, 2);
	m_pMidiPortFilter->portFilterDirection =
		m_pData->at(3) == 1 ? PortDirection::INPUT : PortDirection::OUTPUT;
	m_pMidiPortFilter->numberOfControllerFilters = m_pData->at(4);
	m_pMidiPortFilter->midiControllerFilter = new MIDIControllerFilter
		*[m_pMidiPortFilter->numberOfControllerFilters];
	parseMidiSystemMessagesFilter();
	for (unsigned int i = 0; i < 16; i++) {
		parseMidiChannelMessagesFilter(i);
	}
	for (unsigned int i = 0; i < m_pMidiPortFilter->numberOfControllerFilters;
		 i++) {
		parseMidiControllerFilter(i);
	}
}

void RetSetMidiPortFilter::parseMidiSystemMessagesFilter() {
	long filter = MIDI::byteJoin8bit(m_pData, 5, 2);
	MIDISystemMessagesFilter *midiSystemMessagesFilter =
		new MIDISystemMessagesFilter();
	midiSystemMessagesFilter->filterMidiResetEvents = filter & 256;
	midiSystemMessagesFilter->filterMidiActiveSensingEvents = filter & 64;
	midiSystemMessagesFilter->filterMidiRealtimeEvents = filter & 32;
	midiSystemMessagesFilter->filterMidiTuneRequestEvents = filter & 16;
	midiSystemMessagesFilter->filterMidiSongSelectEvents = filter & 8;
	midiSystemMessagesFilter->filterMidiSongPositionPointerEvents = filter & 4;
	midiSystemMessagesFilter->filterMidiTimeCodeEvents = filter & 2;
	midiSystemMessagesFilter->filterMidiSysExEvents = filter & 1;
	m_pMidiPortFilter->midiSystemMessagesFilter = midiSystemMessagesFilter;
}

void RetSetMidiPortFilter::parseMidiChannelMessagesFilter(
	unsigned int midiChannel) {
	unsigned int filter = m_pData->at(7 + midiChannel);
	MIDIChannelMessagesFilter *midiChannelMessagesFilter =
		new MIDIChannelMessagesFilter();
	midiChannelMessagesFilter->filterMidiPitchBendEvents = filter & 32;
	midiChannelMessagesFilter->filterMidiChannelPressureEvents = filter & 16;
	midiChannelMessagesFilter->filterMidiProgrammChangeEvents = filter & 8;
	midiChannelMessagesFilter->filterMidiControlChangeEvents = filter & 4;
	midiChannelMessagesFilter->filterMidiPolyKeyPressureEvents = filter & 2;
	midiChannelMessagesFilter->filterMidiNoteOnOffEvents = filter & 1;
	m_pMidiPortFilter->midiChannelMessagesFilter[midiChannel] =
		midiChannelMessagesFilter;
}

void RetSetMidiPortFilter::parseMidiControllerFilter(
	unsigned int contollerFilterNumber) {
	unsigned int datalength = 5;
	unsigned int offset = 23 + (datalength * contollerFilterNumber);
	if (offset + datalength <= m_pData->size()) {
		MIDIControllerFilter *midiControllerFilter = new MIDIControllerFilter();
		unsigned int midiControllerNumber = m_pData->at(offset + 4);
		long midiChannelBitmap = MIDI::byteJoin8bit(m_pData, offset, 4);
		if (debug) {
			std::cout << "Midi Controller Filter" << contollerFilterNumber
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
		m_pMidiPortFilter->midiControllerFilter[contollerFilterNumber] =
			midiControllerFilter;
	}
}

std::vector<unsigned char> *
RetSetMidiPortFilter::createMidiSystemMessagesFilterData() {
	MIDISystemMessagesFilter *filter =
		m_pMidiPortFilter->midiSystemMessagesFilter;
	long filterMatrix = 0;
	if (filter->filterMidiResetEvents)
		filterMatrix += 256;
	if (filter->filterMidiActiveSensingEvents)
		filterMatrix += 64;
	if (filter->filterMidiRealtimeEvents)
		filterMatrix += 32;
	if (filter->filterMidiTuneRequestEvents)
		filterMatrix += 16;
	if (filter->filterMidiSongSelectEvents)
		filterMatrix += 8;
	if (filter->filterMidiSongPositionPointerEvents)
		filterMatrix += 4;
	if (filter->filterMidiTimeCodeEvents)
		filterMatrix += 2;
	if (filter->filterMidiSysExEvents)
		filterMatrix += 1;
	return MIDI::byteSplit8bit(filterMatrix, 2);
}

std::vector<unsigned char> *
RetSetMidiPortFilter::createMidiChannelMessagesFilterData() {
	MIDIChannelMessagesFilter **filters =
		m_pMidiPortFilter->midiChannelMessagesFilter;
	BYTE_VECTOR *data = new BYTE_VECTOR();
	for (int i = 0; i < MIDI_CHANNELS; i++) {
		MIDIChannelMessagesFilter *filter = filters[i];
		unsigned char b = 0;
		if (filter->filterMidiPitchBendEvents)
			b += 32;
		if (filter->filterMidiChannelPressureEvents)
			b += 16;
		if (filter->filterMidiProgrammChangeEvents)
			b += 8;
		if (filter->filterMidiControlChangeEvents)
			b += 4;
		if (filter->filterMidiPolyKeyPressureEvents)
			b += 2;
		if (filter->filterMidiNoteOnOffEvents)
			b += 1;
		data->push_back(b);
	}
	return data;
}

std::vector<unsigned char> *
RetSetMidiPortFilter::createMidiControllerFilterData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	MIDIControllerFilter **filters = m_pMidiPortFilter->midiControllerFilter;
	unsigned int numberOfFilters = sizeof(filters);
	for (unsigned int i = 0; i < numberOfFilters; i++) {
		MIDIControllerFilter *filter = filters[i];
		long iChannelMatrix = 0;
		for (unsigned int midiChannel = 0; midiChannel < MIDI_CHANNELS;
			 midiChannel++) {
			if (filter->channel[midiChannel]) {
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
			static_cast<unsigned char>(filter->midiContollerNumber));
	}
	return data;
}

std::vector<unsigned char> *RetSetMidiPortFilter::m_pGetMessageData() {
	// Write Flag
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *portIdV =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	BYTE_VECTOR *midiSystemMessagesFilterData =
		createMidiSystemMessagesFilterData();
	BYTE_VECTOR *midiChannelMessagesFilterData =
		createMidiChannelMessagesFilterData();
	BYTE_VECTOR *midiControllerMessagesFilterData =
		createMidiControllerFilterData();
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	// Command Version Number
	messageData->push_back(m_iCommandVersionNumber);
	// Portnumber
	messageData->insert(messageData->end(), portIdV->begin(), portIdV->end());
	// Filter Direction
	messageData->push_back(m_pMidiPortFilter->portFilterDirection);
	// Number of Controller Filters
	messageData->push_back(static_cast<unsigned int>(
		sizeof(m_pMidiPortFilter->numberOfControllerFilters)));
	// Midi System Messages Data
	messageData->insert(messageData->end(),
						midiSystemMessagesFilterData->begin(),
						midiSystemMessagesFilterData->end());
	// Midi Channel Messages Data
	messageData->insert(messageData->end(),
						midiChannelMessagesFilterData->begin(),
						midiChannelMessagesFilterData->end());
	// Midi Controller Messages Data
	messageData->insert(messageData->end(),
						midiControllerMessagesFilterData->begin(),
						midiControllerMessagesFilterData->end());
	delete portIdV;
	delete midiSystemMessagesFilterData;
	delete midiChannelMessagesFilterData;
	delete midiControllerMessagesFilterData;
	return messageData;
}
