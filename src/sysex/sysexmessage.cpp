#include "sysexmessage.h"
#include <algorithm>
#include <iostream>
#include <unistd.h>

/*
 * Structure and naming od sysex data
 *
 * SYSEX_START 1Byte
 * MANUFACTURER_CODE 3Byte--| Header
 * MESSAGE_CLASS 1Byte ------
 *
 * PRODUCT_ID 2Byte --------| DeviceId -
 * SERIAL_NUMBER 5 Byte -----          |
 *                                     |
 * TRANSACTION_ID 2Byte ----|          | Body (Relevant for Checksum)
 * COMMAND 2Byte            |          |
 * DATA_LENGTH 2Byte        |          |
 * COMMAND_DATA xByte -------          |
 *                          -----------|
 * X_SUM 1yte
 *
 * SYSEX_END 1Byte
 *
 * ST| Man.Id |MC| PID | Serial No.   | TID | CID | LEN  | Data|CS|SE
 * 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17   ...  .. ..
 * F0 00 01 73 7E xx xx xx xx xx xx xx xx xx xx xx xx xx  xxxx  xx F7
 */

SysExMessage::SysExMessage(Command cmd, CommandFlags flags, Device *device)
    : cmd(cmd), cmdflags(flags), device(device) {
  if (this->device != 0)
    this->deviceHeader = this->device->getDeviceHeader();
  else
    deviceHeader = new BYTE_VECTOR();
  command = new BYTE_VECTOR();
  command->push_back(flags);
  command->push_back(cmd);
  acceptedAnswers = commandAcceptedAnswers[cmd];
}

SysExMessage::SysExMessage(Command cmd, std::vector<unsigned char> *message,
                           Device *device)
    : cmd(cmd), device(device) {
  if (this->device != 0)
    this->deviceHeader = this->device->getDeviceHeader();
  else
    deviceHeader = new BYTE_VECTOR();
  cmdflags = (*message)[14];
  command = new BYTE_VECTOR();
  command->push_back(cmdflags);
  command->push_back(cmd);
  acceptedAnswers = commandAcceptedAnswers[cmd];
  long dataLength = MIDI::byteJoin(
      new BYTE_VECTOR(message->begin() + Device::DATA_LENGTH_OFFSET,
                      message->begin() + Device::DATA_LENGTH_OFFSET +
                          Device::DATA_LENGTH_LENGTH));
  data = new BYTE_VECTOR(message->begin() + Device::DATA_OFFSET,
                         message->begin() + Device::DATA_OFFSET + dataLength);
}

SysExMessage::~SysExMessage() {
  delete command;
  delete deviceHeader;
}

BYTE_VECTOR *SysExMessage::getMIDISysExMessage() {
  BYTE_VECTOR *body = new BYTE_VECTOR();
  BYTE_VECTOR *message = new BYTE_VECTOR();
  BYTE_VECTOR *manufacturerHeader = Device::getManufacturerHeader();

  BYTE_VECTOR *md = getMessageData();
  int mdSize = md->size();
  BYTE_VECTOR *bodyLength = MIDI::byteSplit(mdSize, 2);
  BYTE_VECTOR *transactionId = getTransactionId();

  body->reserve(deviceHeader->size() + 6 + mdSize);
  body->insert(body->end(), deviceHeader->begin(), deviceHeader->end());
  body->insert(body->end(), transactionId->begin(), transactionId->end());
  body->insert(body->end(), getCommand()->begin(), getCommand()->end());
  body->insert(body->end(), bodyLength->begin(), bodyLength->end());
  if (mdSize > 0) {
    body->insert(body->end(), md->begin(), md->end());
  }
  unsigned char cs = MIDI::RolandChecksum(body);

  message->reserve(manufacturerHeader->size() + deviceHeader->size() + mdSize +
                   4);
  message->push_back(SYSEX_START);
  message->insert(message->end(), manufacturerHeader->begin(),
                  manufacturerHeader->end());
  message->push_back(Device::MESSAGE_CLASS);
  message->insert(message->end(), body->begin(), body->end());
  message->push_back(cs);
  message->push_back(SYSEX_END);
  return message;
}

SysExMessage::Command SysExMessage::parseAnswer(BYTE_VECTOR *answer) {
  std::cout << "Answer: " << std::dec << answer->size() << std::endl;
  BYTE_VECTOR *commandBytes =
      new BYTE_VECTOR(answer->begin() + 14, answer->begin() + 16);
  MIDISysexValue command = MIDISysexValue(commandBytes);
  if (checkAnswerValid(command.getLongValue())) {
#ifdef __RTMIDI_DEBUG__
    std::cout << "Answer (command: " << command.getLongValue() << ") accepted "
              << std::endl;
#endif //__RTMIDI_DEBUG__
    return (SysExMessage::Command)command.getLongValue();
  }
#ifdef __RTMIDI_DEBUG__
  std::cout << "Answer (command: " << command.getLongValue()
            << ") not accepted " << std::endl;
#endif //__RTMIDI_DEBUG__
  return CMD_ERROR;
}

bool SysExMessage::checkAnswerValid(long answerCommandId) {
  return std::find(acceptedAnswers.begin(), acceptedAnswers.end(),
                   answerCommandId) != acceptedAnswers.end();
}

int SysExMessage::execute() {
  if (device == 0)
    return -1;
  BYTE_VECTOR *message = getMIDISysExMessage();
  device->sentSysex(message);
  BYTE_VECTOR *answerMessage = device->retrieveSysex();
  if( answerMessage != nullptr  ){
    Command cmd = parseAnswer(answerMessage);
    if (debug)
      MIDI::printMessage(answerMessage);
    if (cmd != this->cmd)
      createAnswer(cmd, answerMessage, device);
  }
  return 0;
}

void SysExMessage::setDebug(bool debug) { this->debug = debug; }

void SysExMessage::printRawData() { MIDI::printMessage(data); }

SysExMessage *SysExMessage::getAnswer() { return answer; }

SysExMessage *SysExMessage::query() {
  execute();
  return answer;
}

std::string SysExMessage::getDataAsString() {
  std::string string2(data->begin(), data->end());
  return string2;
}

long SysExMessage::getDataAsLong() {
  long result = -1;
  if (data->size() < 11) {
    result = MIDI::byteJoin(data);
  }
  return result;
}

CommandAcceptedAnswers SysExMessage::commandAcceptedAnswers = {
    {GET_DEVICE, AcceptedAnswers{RET_DEVICE}},
    {GET_COMMAND_LIST, AcceptedAnswers{RET_COMMAND_LIST}},
    {GET_INFO_LIST, AcceptedAnswers{RET_INFO_LIST}},
    {GET_DEVICE_INFO, AcceptedAnswers{RET_SET_DEVICE_INFO}},
    {RET_SET_DEVICE_INFO, AcceptedAnswers{ACK}},
    {GET_RESET_LIST, AcceptedAnswers{RET_RESET_LIST, ACK}},
    {GET_SAVE_RESTORE_LIST, AcceptedAnswers{RET_SAVE_RESTORE_LIST, ACK}},
    {GET_ETHERNET_PORT_INFO, AcceptedAnswers{RET_SET_ETHERNET_PORT_INFO, ACK}},
    {RESET, AcceptedAnswers{ACK}},
    {SAVE_RESTORE, AcceptedAnswers{ACK}},
    {GET_GIZMO_COUNT, AcceptedAnswers{RET_GIZMO_COUNT, ACK}},
    {GET_GIZMO_INFO, AcceptedAnswers{RET_GIZMO_INFO, ACK}},
    {GET_MIDI_INFO, AcceptedAnswers{RET_SET_MIDI_INFO, ACK}},
    {GET_MIDI_PORT_INFO, AcceptedAnswers{RET_SET_MIDI_PORT_INFO}},
    {RET_SET_MIDI_PORT_INFO, AcceptedAnswers{ACK}}};
