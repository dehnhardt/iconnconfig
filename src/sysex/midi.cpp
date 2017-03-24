#include "midi.h"
#include <algorithm>
#include <iostream>

MIDI::MIDI() {}

RtMidiIn *MIDI::createMidiIn(
    const std::string clientName = std::string("RtMidi Input Client")) {
  // RtMidiIn constructor
  RtMidiIn *midiin = 0;
  try {
    midiin = new RtMidiIn(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    // Handle the exception here
    error.printMessage();
  }
  midiin->ignoreTypes(false, true, true);
  return midiin;
}

RtMidiOut *
MIDI::createMidiOut(const std::string clientName = std::string(
                        "RtMidi Output Client")) { // RtMidiOut constructor
  RtMidiOut *midiout = 0;
  try {
    midiout = new RtMidiOut(RtMidi::LINUX_ALSA);
  } catch (RtMidiError &error) {
    // Handle the exception here
    error.printMessage();
  }
  return midiout;
}

unsigned char MIDI::RolandChecksum(std::vector<unsigned char> *message) {
  unsigned int nBytes = message->size();
  unsigned int sum = 0;
  for (unsigned int i = 0; i < nBytes; i++) {
    sum += (int)message->at(i);
    if (sum > 127)
      sum -= 128;
  }
  return 128 - sum;
}

std::vector<unsigned char> *MIDI::byteSplit(long val) {
  return MIDI::byteSplit(val, 0);
}

std::vector<unsigned char> *MIDI::byteSplit(long val, int size) {
  std::vector<unsigned char> *bytes = new std::vector<unsigned char>();
  while (val > 0) {
    unsigned char c = val & 0x7f;
    val >>= 7;
    bytes->push_back(c);
  }
  if (size > 0) {
    int nLength = bytes->size();
    if (nLength < size) {
      for (int i = nLength; i < size; i++)
        bytes->push_back(0x00);
    }
  }
  std::reverse(bytes->begin(), bytes->end());
  return bytes;
}

long MIDI::byteJoin(std::vector<unsigned char> *message) {
  return byteJoin(message, 0, message->size());
}

long MIDI::byteJoin(std::vector<unsigned char> *message, unsigned int start,
                    unsigned int length) {
  unsigned int cnt;
  int current = 0;

  if (start + length >= message->size())
    return -1;

  for (cnt = start; cnt < start + length; cnt++) {
    current <<= 7;
    current += message->at(cnt);
  }
  return current;
}
