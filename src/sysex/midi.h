#ifndef MIDI_H
#define MIDI_H

#include <cstring>
#include <memory>
#include <vector>

#include "../RtMidi.h"

// Platform-dependent sleep routines.
#if defined(__WINDOWS_MM__)
#include <windows.h>
#define uint16_t u_int16_t
#define SLEEP(milliseconds) Sleep((DWORD)milliseconds)
#else // Unix variants
#include <unistd.h>
#define SLEEP(milliseconds)                                                    \
	usleep(static_cast<unsigned long>(milliseconds * 1000.0))
#endif

#ifdef __LINUX_ALSA__
#include <alsa/seq_event.h>
#endif //__LINUX_ALSA__

#define SYSEX_START 0xf0
#define SYSEX_END 0xf7
#define WAIT_TIME 10
#define WAIT_LOOPS 20
#define MIDI_CHANNELS 16

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
	static BYTE_VECTOR *byteSplit7bit(unsigned long val, unsigned long length);
	static BYTE_VECTOR *byteSplit7bit(unsigned long val);
	static BYTE_VECTOR *byteSplit8bit(long val, unsigned long length);
	static BYTE_VECTOR *byteSplit8bit(long val);
	static long
	byteJoin7bit(std::shared_ptr<std::vector<unsigned char>> message,
				 unsigned long start, unsigned long length);
	static long
	byteJoin7bit(std::shared_ptr<std::vector<unsigned char>> message);
	static long byteJoin7bit(BYTE_VECTOR *message);
	static long byteJoin7bit(BYTE_VECTOR *message, unsigned long start,
							 unsigned long end);
	static long
	byteJoin8bit(std::shared_ptr<std::vector<unsigned char>> message,
				 unsigned long start, unsigned long length);
	static long
	byteJoin8bit(std::shared_ptr<std::vector<unsigned char>> message);
	static long byteJoin8bit(BYTE_VECTOR *message);
	static long byteJoin8bit(BYTE_VECTOR *message, unsigned long start,
							 unsigned long end);

	static std::string decodeIp(BYTE_VECTOR *data, unsigned long offset);
	static BYTE_VECTOR *encodeIpAddress(std::string ipAddress);
	static void printMessage(BYTE_VECTOR *v);
	static std::string printMessageToHexString(BYTE_VECTOR *message);
	static bool compareByteVector(BYTE_VECTOR *v1, BYTE_VECTOR *v2) {
		if (v1->size() != v2->size())
			return false;
		unsigned long size = v1->size();
		for (unsigned long i = 0; i < size; i++) {
			if ((*v1)[i] != (*v2)[i])
				return false;
		}
		return true;
	}

	static int16_t calc2sComplement(uint16_t data) {
		return (static_cast<int16_t>(data));
	}

	static uint16_t inv2sComplement(int16_t data) {
		return (static_cast<uint16_t>(data));
	}

	// static device specific methods

	static int bytesToSignedInt(std::shared_ptr<BYTE_VECTOR> v,
								unsigned long offset, unsigned long bytes) {
		return bytesToSignedInt(v.get(), offset, bytes);
	}

	static int bytesToSignedInt(BYTE_VECTOR *v, unsigned long offset,
								unsigned long bytes) {
		u_int16_t minValueU = static_cast<u_int16_t>(
			MIDI::byteJoin7bit(v, offset, bytes) & 0xFFFF);
		return MIDI::calc2sComplement(minValueU);
	}

	static RtMidiIn *createMidiIn(
		const std::string clientName = std::string("MioConfig Input Client"));
	static RtMidiOut *createMidiOut(
		const std::string clientName = std::string("MioConfig Output Client"));
	static std::string
	decodeIp(std::shared_ptr<std::vector<unsigned char>> data,
			 unsigned long offset);
};

class MIDISysexValue {
  public:
	MIDISysexValue(long val, unsigned long length)
		: m_iLongValue(val), m_iByteLength(length) {}
	MIDISysexValue(long val) : m_iLongValue(val) {}
	// TODO setting the byte value does not set the correct long value
	MIDISysexValue(BYTE_VECTOR *val) {
		m_pByteValue = val;
		m_iLongValue = MIDI::byteJoin7bit(m_pByteValue);
	}
	~MIDISysexValue() {
		if (m_pByteValue != nullptr)
			delete m_pByteValue;
	}
	long getLongValue() { return m_iLongValue; }
	BYTE_VECTOR *getByteValue() {
		if (m_pByteValue == nullptr)
			m_pByteValue = MIDI::byteSplit7bit(
				static_cast<unsigned long>(m_iLongValue), m_iByteLength);
		return m_pByteValue;
	}

  private:
	long m_iLongValue;
	BYTE_VECTOR *m_pByteValue = nullptr;
	unsigned long m_iByteLength;
};

#endif // MIDI_H
