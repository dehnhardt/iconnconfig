#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../device.h"
#include "midi.h"

#include <vector>

class SysExMessage {
public:
  enum Command {
    GET_DEVICE = 0x01, /*!< Query for deivices  */
    RET_DEVICE,        /*!< Answer from device to GET_DEVICE query*/
    GET_COMMAND_LIST,  /*!< Query devices command list. */
    RET_COMMAND_LIST,  /*!< List of commands supported by device. */
    GET_INFO_LIST,     /*!< Query list of infos, whch can be retrieved */
    RET_INFO_LIST,     /*!< List of infos supported by this device */
    /* ... */
    INVALID_DEVICE,    /*!< An invalid device ID was specified. */
    MEMORY_ERROR,      /*!< An error occured during memory allocation. */
    INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
    INVALID_USE,       /*!< The function was called incorrectly. */
    DRIVER_ERROR,      /*!< A system driver error occured. */
    SYSTEM_ERROR,      /*!< A system error occured. */
    THREAD_ERROR       /*!< A thread error occured. */
  };

  enum CommandFlags {
    ANSWER = 0x00, /*!< Answer from device */
    QUERY = 0x40   /*!< Query from device */
  };

  SysExMessage(Command command, CommandFlags flags, Device *device);
  ~SysExMessage();

  virtual BYTE_VECTOR *getCommand() { return command; }
  virtual BYTE_VECTOR *getMessageData() { return new BYTE_VECTOR(); }
  virtual BYTE_VECTOR *getTransactionId() {
    if (transactionId == 0) {
      transactionId = new BYTE_VECTOR();
      transactionId->push_back(0x00);
      transactionId->push_back(0x01);
    }
    return transactionId;
  }
  virtual BYTE_VECTOR *getMIDISysExMessage();

protected:
  unsigned char cmd;
  unsigned char cmdflags;
  Device *device = 0;
  BYTE_VECTOR *command = 0;
  BYTE_VECTOR *transactionId = 0;
  BYTE_VECTOR *deviceHeader = 0;
};

#endif // SYSEXMESSAGE_H
