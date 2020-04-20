#ifndef RETSETMIDIPORTFILTER_H
#define RETSETMIDIPORTFILTER_H

#include "sysexmessage.h"

typedef struct {
	bool filterMidiResetEvents;
	bool filterMidiActiveSensingEvents;
	bool filterMidiRealtimeEvents;
	bool filterMidiTuneRequestEvents;
	bool filterMidiSongSelectEvents;
	bool filterMidiSongPositionPointerEvents;
	bool filterMidiTimeCodeEvents;
	bool filterMidiSysExEvents;
} MIDISystemMessagesFilter;

typedef struct {
	bool filterMidiPitchBendEvents;
	bool filterMidiChannelPressureEvents;
	bool filterMidiProgrammChangeEvents;
	bool filterMidiControlChangeEvents;
	bool filterMidiPolyKeyPressureEvents;
	bool filterMidiNoteOnOffEvents;
} MIDIChannelMessagesFilter;

typedef struct {
	unsigned int midiContollerNumber;
	bool channel[MIDI_CHANNELS];
} MIDIControllerFilter;

typedef struct {
	unsigned int numberOfControllerFilters;
	pk::PortDirection portFilterDirection;
	MIDISystemMessagesFilter *midiSystemMessagesFilter;
	MIDIChannelMessagesFilter *midiChannelMessagesFilter[MIDI_CHANNELS];
	MIDIControllerFilter **midiControllerFilter;
} MIDIPortFilter;

class RetSetMidiPortFilter : public SysExMessage {
  public:
	RetSetMidiPortFilter(Device *device);
	RetSetMidiPortFilter(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	~RetSetMidiPortFilter();

  public: // Methods
	void parseAnswerData();
	void parseMidiSystemMessagesFilter();
	void parseMidiChannelMessagesFilter(unsigned int midiChannel);
	void parseMidiControllerFilter(unsigned int contollerFilterNumber);

	BYTE_VECTOR *createMidiSystemMessagesFilterData();
	BYTE_VECTOR *createMidiChannelMessagesFilterData();
	BYTE_VECTOR *createMidiControllerFilterData();

	BYTE_VECTOR *getMessageData();

  public: // Getter and Setter
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
	MIDIPortFilter *getMidiPortFilter() { return m_pMidiPortFilter; }
	void setMidiPortFilter(MIDIPortFilter *midiPortFilter) {
		m_pMidiPortFilter = midiPortFilter;
	}

  private:
	long m_iPortId;
	MIDIPortFilter *m_pMidiPortFilter = nullptr;
};

#endif // RETSETMIDIPORTFILTER_H
