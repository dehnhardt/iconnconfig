#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../device.h"
#include "midi.h"

#include <vector>

class SysExMessage {
public:
  enum Command {
    GET_DEVICE = 0x01, /*!< Query for devices  */
    RET_DEVICE,        /*!< Answer from device to GET_DEVICE query*/
    GET_COMMAND_LIST,  /*!< Request devices command list. */
    RET_COMMAND_LIST,  /*!< List of commands supported by device. */
    GET_INFO_LIST,     /*!< Request list of infos, whch can be retrieved */
    RET_INFO_LIST,     /*!< List of infos supported by this device */
    GET_INFO,          /*!< Get a single information from device */
    RET_SET_INFO,      /*!< Retrun a single information from device see
                          DeviceInfoItem enum in Info-Class*/
    GET_RESET_LIST, /*!< Request list of reset IDs or an ACK if no reset ids are
                       supported */
    RET_RESET_LIST  /*!< Return list of reset IDs see ResetListItem enum in
                       Reset class*/

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
      if (device != 0)
        return this->device->nextTransactionId();
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
