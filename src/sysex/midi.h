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
  static void printMessage(BYTE_VECTOR *v);
  static bool compareByteVector(BYTE_VECTOR *v1, BYTE_VECTOR *v2) {
    if (v1->size() != v2->size())
      return false;
    int size = v1->size();
    for (int i = 0; i < size; i++) {
      if ((*v1)[i] != (*v2)[i])
        return false;
    }
    return true;
  }

  static RtMidiIn *createMidiIn(
      const std::string clientName = std::string("MioConfig Input Client"));
  static RtMidiOut *createMidiOut(
      const std::string clientName = std::string("MioConfig Output Client"));
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
