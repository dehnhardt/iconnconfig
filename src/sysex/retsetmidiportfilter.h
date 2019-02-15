#ifndef RETSETMIDIPORTFILTER_H
#define RETSETMIDIPORTFILTER_H

#include "sysexmessage.h"

class RetSetMidiPortFilter : public SysExMessage {
  public:
	RetSetMidiPortFilter(Device *device);
};

#endif // RETSETMIDIPORTFILTER_H
