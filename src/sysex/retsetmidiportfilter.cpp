#include "retsetmidiportfilter.h"
#include <cmath>

RetSetMidiPortFilter::RetSetMidiPortFilter(Device *device)
	: SysExMessage(Command::RET_SET_MIDI_PORT_FILTER, SysExMessage::QUERY,
				   device) {}

RetSetMidiPortFilter::~RetSetMidiPortFilter() { delete m_pMidiPortFilter; }

void RetSetMidiPortFilter::parseAnswerData() {
	if (!m_pMidiPortFilter)
		m_pMidiPortFilter = new MIDIPortFilter();
	m_iCommandVersion = m_pData->at(0);
	m_iPortId = MIDI::byteJoin(m_pData, 1, 2);
	m_portFilterDirection = m_pData->at(3) == 1 ? PortFilterDirection::INPUT
												: PortFilterDirection::OUTPUT;
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
	long filter = MIDI::byteJoin(m_pData, 5, 2);
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
	unsigned int filter = m_pData->at(6 + midiChannel);
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
	unsigned int offset = 22 + 5 * contollerFilterNumber;
	if (offset + 5 < m_pData->size()) {
		MIDIControllerFilter *midiControllerFilter = new MIDIControllerFilter();
		unsigned int midiControllerNumber = m_pData->at(offset);
		long midiChannelBitmap = MIDI::byteJoin(m_pData, offset + 1, 4);
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
			midiControllerFilter->channel[i] =
				midiChannelBitmap &
				static_cast<long>(
					pow(2.0, static_cast<double>(midiChannelMask)));
		}
		m_pMidiPortFilter->midiControllerFilter[contollerFilterNumber] =
			midiControllerFilter;
	}
}

std::vector<unsigned char> *RetSetMidiPortFilter::m_pGetMessageData() {
	this->m_pCommandData->at(0) = 0x40;
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	/*messageData->push_back(0x01);
	BYTE_VECTOR *portIdV =
		MIDI::byteSplit(static_cast<unsigned long>(m_iPortId), 2);
	messageData->insert(messageData->end(), portIdV->begin(), portIdV->end());
	messageData->insert(messageData->end(), m_pPortRoutings->begin(),
						m_pPortRoutings->end());*/
	return messageData;
}
