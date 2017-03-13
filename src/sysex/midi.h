#ifndef MIDI_H
#define MIDI_H

#include <cstring>
#include <vector>

#ifdef __LINUX_ALSA__
#include <alsa/seq_event.h>
#endif //__LINUX_ALSA__

#define SYSEX_START 0xf0
#define SYSEX_END 0xff

class MIDI
{
public:
    MIDI();

public:
    //Helper Methods
    static unsigned char RolandChecksum( std::vector< unsigned char > *message );
    static std::vector<unsigned char> * byteSplit( long val );
    static long byteJoin( std::vector< unsigned char > *message );
    static long byteJoin( std::vector< unsigned char > *message, unsigned int start, unsigned int end );

#ifdef __LINUX_ALSA__
    enum midiEventType {
        MIDI_EVENT_SYSEX = SND_SEQ_EVENT_SYSEX
    };
#endif // __LINUX_ALSA__
};

#endif // MIDI_H
