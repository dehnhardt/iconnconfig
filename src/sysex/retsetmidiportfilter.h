#ifndef RETSETMIDIPORTFILTER_H
#define RETSETMIDIPORTFILTER_H

#include "sysexmessage.h"

enum PortFilterDirection { INPUT = 1, OUTPUT };

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

  public: // Getter and Setter
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
	BYTE_VECTOR *m_pGetMessageData();
	MIDIPortFilter *getMidiPortFilter() { return m_pMidiPortFilter; }
	void setMidiPortFilter(MIDIPortFilter *midiPortFilter) {
		m_pMidiPortFilter = midiPortFilter;
	}

  private:
	long m_iPortId;
	unsigned int m_iCommandVersion;
	PortFilterDirection m_portFilterDirection;
	MIDIPortFilter *m_pMidiPortFilter = nullptr;
};

#endif // RETSETMIDIPORTFILTER_H
