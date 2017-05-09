#ifndef INFOS_H
#define INFOS_H

#include "sysexmessage.h"

class GetInfoList : public SysExMessage {
public:
	GetInfoList(Device *device);
	GetInfoList(SysExMessage::Command cmd, BYTE_VECTOR *message, Device *device)
      : SysExMessage(cmd, message, device) {}

private:
  void createAnswer(SysExMessage::Command cmd, BYTE_VECTOR *message,
                    Device *device) {
		answer = new GetInfoList(cmd, message, device);
  }
};

#endif // INFOS_H
