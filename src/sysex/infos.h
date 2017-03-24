#ifndef INFOS_H
#define INFOS_H

#include "sysexmessage.h"

class Infos : public SysExMessage {
public:
  Infos(Device *device);
};

#endif // INFOS_H
