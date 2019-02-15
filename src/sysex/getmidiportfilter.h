#ifndef GETMIDIPORTFILTER_H
#define GETMIDIPORTFILTER_H

#include "sysexmessage.h"

class GetMidiPortFilter : public SysExMessage {
  public:
	GetMidiPortFilter(Device *device);
};

#endif // GETMIDIPORTFILTER_H
