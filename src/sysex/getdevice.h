#ifndef DEVICEDETECTIONQUERY_H
#define DEVICEDETECTIONQUERY_H

#include "midi.h"
#include "sysexmessage.h"

class GetDevice : public SysExMessage {
public:
  GetDevice();
	GetDevice(Device *device);
	~GetDevice();
};

#endif // DEVICEDETECTIONQUERY_H
