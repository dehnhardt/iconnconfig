#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "midi.h"
#include "sysexmessage.h"

class Commands : public SysExMessage {
public:
  Commands(Device *devices);
};

#endif // GETCOMMANDS_H