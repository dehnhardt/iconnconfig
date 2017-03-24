#ifndef GETCOMMANDS_H
#define GETCOMMANDS_H

#include "sysexmessage.h"

class Commands : public SysExMessage {
public:
  Commands(Device *device);
};

#endif // GETCOMMANDS_H
