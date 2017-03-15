#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../device.h"
#include "midi.h"

#include <vector>

class SysExMessage {
public:
  SysExMessage(BYTE_VECTOR *deviceHeader);
  ~SysExMessage();
  virtual BYTE_VECTOR *getCommand() { return command; }
  virtual BYTE_VECTOR *getMessageData() { return 0; }
  virtual BYTE_VECTOR *getTransactionId() {
    if (transactionId == 0) {
      transactionId = new BYTE_VECTOR();
      transactionId->push_back(0x00);
      transactionId->push_back(0x00);
    }
    return transactionId;
  }
  virtual BYTE_VECTOR *getMIDISysExMessage();

protected:
  BYTE_VECTOR *command = 0;
  BYTE_VECTOR *transactionId = 0;
  BYTE_VECTOR *deviceHeader = 0;
};

#endif // SYSEXMESSAGE_H
