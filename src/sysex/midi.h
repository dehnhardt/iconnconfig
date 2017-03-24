#ifndef MIDI_H
#define MIDI_H

#include <cstring>
#include <vector>

#include "../RtMidi.h"

#ifdef __LINUX_ALSA__
#include <alsa/seq_event.h>
#endif //__LINUX_ALSA__

#define SYSEX_START 0xf0
#define SYSEX_END 0xf7

#define BYTE_VECTOR std::vector<unsigned char>
#ifdef __LINUX_ALSA__
enum midiEventType { MIDI_EVENT_SYSEX = SND_SEQ_EVENT_SYSEX };
#endif // __LINUX_ALSA__

class MIDI {
public:
  MIDI();

public:
  // Helper Methods
  static unsigned char RolandChecksum(std::vector<unsigned char> *message);
  static BYTE_VECTOR *byteSplit(long val, int length);
  static BYTE_VECTOR *byteSplit(long val);
  static long byteJoin(BYTE_VECTOR *message);
  static long byteJoin(BYTE_VECTOR *message, unsigned int start,
                       unsigned int end);
  static RtMidiIn *createMidiIn(
      const std::string clientName = std::string("RtMidi Input Client"));
  static RtMidiOut *createMidiOut(
      const std::string clientName = std::string("RtMidi Input Client"));
};

class MIDISysexValue {
public:
  MIDISysexValue(long val, int length) : longValue(val), byteLength(length) {}
  MIDISysexValue(long val) : longValue(val) {}
  // TODO setting the byte value does not set the correct long value
  MIDISysexValue(BYTE_VECTOR *val) {
    byteValue = val;
    longValue = MIDI::byteJoin(byteValue);
  }

  long getLongValue() { return longValue; }
  BYTE_VECTOR *getByteValue() {
    if (byteValue == 0)
      byteValue = MIDI::byteSplit(longValue, byteLength);
    return byteValue;
  }

private:
  long longValue;
  BYTE_VECTOR *byteValue = 0;
  int byteLength;
};

#endif // MIDI_H
