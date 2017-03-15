#ifndef DEVICEDETECTIONQUERY_H
#define DEVICEDETECTIONQUERY_H

#include "midi.h"
#include "sysexmessage.h"

class DeviceDetectionQuery : public SysExMessage {
public:
  DeviceDetectionQuery();
  ~DeviceDetectionQuery();

  // BYTE_VECTOR *getMIDISysExMessage();
  BYTE_VECTOR *getMessageData();

private:
  BYTE_VECTOR *messageData = 0;
};

#endif // DEVICEDETECTIONQUERY_H
