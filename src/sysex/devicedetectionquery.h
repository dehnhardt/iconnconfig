#ifndef DEVICEDETECTIONQUERY_H
#define DEVICEDETECTIONQUERY_H

#include "midi.h"
#include "sysexmessage.h"

class DeviceDetectionQuery : public SysExMessage {
public:
  DeviceDetectionQuery();
  ~DeviceDetectionQuery();
};

#endif // DEVICEDETECTIONQUERY_H
