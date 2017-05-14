#ifndef ACK_H
#define ACK_H

#include "sysexmessage.h"

class Ack : public SysExMessage {
public:
  Ack(Command cmd, BYTE_VECTOR *message, Device *device);
  void checkResult();

protected:
  void parseAnswerData();

private:
  unsigned char result;
};

#endif // ACK_H
