#ifndef SYSEXMESSAGE_H
#define SYSEXMESSAGE_H

#include "../device.h"
#include "midi.h"

#include <map>
#include <vector>

typedef std::vector<int> AcceptedAnswers;
typedef std::map<int, AcceptedAnswers> CommandAcceptedAnswers;

/**
 * @brief The SysExMessage class
 */
class SysExMessage {

  // enums
public:
  /**
 * @brief The Command enum
 */
  enum Command {
    CMD_ERROR = -1,      /*!< Return if command is not valid */
    GET_DEVICE = 0x01,   /*!< Query for devices  */
    RET_DEVICE,          /*!< Answer from device to GET_DEVICE query*/
    GET_COMMAND_LIST,    /*!< Request devices command list. */
    RET_COMMAND_LIST,    /*!< List of commands supported by device. */
    GET_INFO_LIST,       /*!< Request list of infos, whch can be retrieved */
    RET_INFO_LIST,       /*!< List of infos supported by this device */
    GET_DEVICE_INFO,     /*!< Get a single information from device */
    RET_SET_DEVICE_INFO, /*!< Retrun a single information from device see
                     DeviceInfoItem enum in Info-Class*/
    GET_RESET_LIST, /*!< Request list of reset IDs or an ACK if no reset ids are
                       supported */
    RET_RESET_LIST, /*!< Return list of reset IDs see ResetListItem enum in
                      Reset class*/
    GET_SAVE_RESTORE_LIST,  /*!< Request the save / restore ids of the device */
    RET_SAVE_RESTORE_LIST,  /*!< Returns a list of the save / restore ids of the
                               device */
    GET_ETHERNET_PORT_INFO, /*!< Requests the information about a specific
                               ethernet port */
    RET_SET_ETHERNET_PORT_INFO, /*!< Returns the information about a specifoc
                                   ethernet port or sets some values for a
                                   specifoc ethernet port */
    ACK /*!< Returns a sucess code (0x00) or an error code (0x01 - 0x03) */

  };

  enum DeviceInfoItem {
    ACESSORY_NAME = 0x01, /*!< Name of acessory */
    MANUFACTURER_NAME,    /*!< Name of device manufacturer manufacturer */
    MODEL_NUMBER,         /*!< Devices modelnumber */
    SERIAL_NUMBER,        /*!< Devices serial number */
    FIRMWARE_VERSION,     /*!< Installed firmware version */
    HARDWARE_VERSION,     /*!< Hardware version of device */
    DEVICE_NAME = 0x10    /*!< Name of device (writable) */
  };

  enum CommandFlags {
    ANSWER = 0x00, /*!< Answer from device */
    QUERY = 0x40   /*!< Query from device */
  };

  enum Errors {
    NO_DEVICE = -1, /*!< No device found or assigned */
    OK = 0          /*!< No error */
  };

  static CommandAcceptedAnswers commandAcceptedAnswers;

  // methods
public:
  SysExMessage(Command cmd, CommandFlags flags, Device *device);
  SysExMessage(Command cmd, BYTE_VECTOR *message, Device *device);

  ~SysExMessage();
  virtual BYTE_VECTOR *getMIDISysExMessage();
  std::string getDataAsString();
  long getDataAsLong();
  Command parseAnswer(BYTE_VECTOR *answer);
  SysExMessage *getAnswer();
  int execute();

  // methods
protected:
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
  bool checkAnswerValid(long answerCommandId);
  virtual void createAnswer(SysExMessage::Command cmd __attribute__((unused)),
                            BYTE_VECTOR *message __attribute__((unused)),
                            Device *device __attribute__((unused))) {}

  // members
protected:
  unsigned char cmd;
  unsigned char cmdflags;
  AcceptedAnswers acceptedAnswers;
  Device *device = 0;
  SysExMessage *answer;
  BYTE_VECTOR *command = 0;
  BYTE_VECTOR *transactionId = 0;
  BYTE_VECTOR *deviceHeader = 0;
  // BYTE_VECTOR *answer = 0;
  BYTE_VECTOR *data = 0;
};

#endif // SYSEXMESSAGE_H
