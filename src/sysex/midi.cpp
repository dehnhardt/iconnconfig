#include "midi.h"
#include <iostream>
#include <algorithm>

MIDI::MIDI()
{

}

unsigned char MIDI::RolandChecksum( std::vector< unsigned char > *message ){
    unsigned int nBytes = message->size();
    unsigned int sum = 0;
    for ( unsigned int i=0; i<nBytes; i++ ){
    sum += (int) message->at(i);
    if( sum > 127 )
        sum -= 128;
        }
    return 128 - sum;
}

std::vector<unsigned char> * MIDI::byteSplit( long val ){
    std::vector<unsigned char> * bytes = new std::vector<unsigned char>();
    while( val > 0 ){
        unsigned char c = val & 0x7f;
        val >>= 7;
        bytes->push_back(c);
    }
    std::reverse(bytes->begin(), bytes->end());
    return bytes;
}

long MIDI::byteJoin( std::vector< unsigned char > *message ){
    return byteJoin(message, 0, message->size());
}

long MIDI::byteJoin( std::vector< unsigned char > *message, unsigned int start, unsigned int length ){
    unsigned int cnt;
    int current = 0;

    if( start+length >= message->size() )
        return -1;

    for (cnt = start; cnt < start+length; cnt++) {
        current <<= 7;
        current += message->at(cnt);
      }
      return current;
}
