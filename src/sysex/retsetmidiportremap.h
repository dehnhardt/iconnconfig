#ifndef RETSETMIDIPORTREMAP_H
#define RETSETMIDIPORTREMAP_H

#include "sysexmessage.h"

enum PortRemapDirection { INPUT = 1, OUTPUT };

typedef struct {
    bool remapMidiPitchBendEvents;
    bool remapMidiChannelPressureEvents;
    bool remapMidiProgrammChangeEvents;
    bool remapMidiControlChangeEvents;
    bool remapMidiPolyKeyPressureEvents;
    bool remapMidiNoteOnOffEvents;
} MIDIChannelMessagesRemap;

typedef struct {
	unsigned int midiContollerNumber;
	bool channel[MIDI_CHANNELS];
} MIDIControllerRemap;

typedef struct {
    unsigned int numberOfControllerRemaps;
    PortRemapDirection portRemapDirection;
    MIDIChannelMessagesRemap *midiChannelMessagesRemap[MIDI_CHANNELS];
    MIDIControllerRemap **midiControllerRemap;
} MIDIPortRemap;

class RetSetMidiPortRemap : public SysExMessage {
  public:
    RetSetMidiPortRemap(Device *device);
    RetSetMidiPortRemap(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
    ~RetSetMidiPortRemap();

  public: // Methods
	void parseAnswerData();
    void parseMidiChannelMessagesRemap(unsigned int midiChannel);
    void parseMidiControllerRemap(unsigned int contollerRemapNumber);

    BYTE_VECTOR *createMidiChannelMessagesRemapData();
    BYTE_VECTOR *createMidiControllerRemapData();

	BYTE_VECTOR *m_pGetMessageData();

  public: // Getter and Setter
	int getSettingsId() { return m_Command; }
	int getSettingsIndex() { return 0; }
	std::string getStorableValue() { return ""; }
    MIDIPortRemap *getMidiPortRemap() { return m_pMidiPortRemap; }
    void setMidiPortRemap(MIDIPortRemap *midiPortRemap) {
        m_pMidiPortRemap = midiPortRemap;
	}

  private:
	long m_iPortId;
	unsigned int m_iCommandVersion;
    MIDIPortRemap *m_pMidiPortRemap = nullptr;
};

#endif // RETSETMIDIPORTREMAP_H
