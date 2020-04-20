#ifndef RETSETMIDIPORTREMAP_H
#define RETSETMIDIPORTREMAP_H

#include "sysexmessage.h"

typedef struct {
	bool remapMidiPitchBendEvents;
	bool remapMidiChannelPressureEvents;
	bool remapMidiProgrammChangeEvents;
	bool remapMidiControlChangeEvents;
	bool remapMidiPolyKeyPressureEvents;
	bool remapMidiNoteOnOffEvents;
	unsigned int remapChannel;
} MIDIChannelMessagesRemap;

typedef struct {
	unsigned int midiContollerSourceNumber;
	unsigned int midiContollerDestinationNumber;
	bool channel[MIDI_CHANNELS];
} MIDIControllerRemap;

typedef struct {
	unsigned int numberOfControllerRemaps;
	pk::PortDirection portRemapDirection;
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

	BYTE_VECTOR *getMessageData();

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
	MIDIPortRemap *m_pMidiPortRemap = nullptr;
};

#endif // RETSETMIDIPORTREMAP_H
