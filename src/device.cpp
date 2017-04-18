#include "device.h"
#include "sysex/commands.h"
#include "sysex/deviceinfo.h"
#include "sysex/getcommands.h"
#include "sysex/infos.h"
#include "sysex/midi.h"

#include <array>
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>

Device::Device(int inPortNumber, int outPortNumber, long serialNumber,
               int productId) {
  this->inPortNumber = inPortNumber;
  this->outPortNumber = outPortNumber;
  this->serialNumber = new MIDISysexValue(serialNumber, 5);
  this->productId = new MIDISysexValue(productId, 2);
  setupMidi();
}

#ifdef __MIO_SIMULATE__

Device::Device(int inPortNumber, int outPortNumber, long serialNumber,
               int productId, std::string modelName, std::string deviceName) {
  this->inPortNumber = inPortNumber;
  this->outPortNumber = outPortNumber;
  this->serialNumber = new MIDISysexValue(serialNumber, 5);
  this->productId = new MIDISysexValue(productId, 2);
  this->modelName = modelName;
  this->deviceName = deviceName;
  this->deviceIsSimulated = true;
}

Commands *Device::simulateCommands(Commands *command) {
  BYTE_VECTOR *message = new BYTE_VECTOR({0xF0});
  message->insert(message->end(), getFullHeader()->begin(),
                  getFullHeader()->end());
  message->push_back(0x00);
  message->push_back(0x01);
  message->push_back(0x00);
  message->push_back(SysExMessage::RET_COMMAND_LIST);
  message->push_back(0x00);
  message->push_back(0x06);
  message->insert(message->end(), {0x05, 0x07, 0x08, 0x09, 0x0b, 0x0c});
  commands = new Commands(SysExMessage::RET_COMMAND_LIST, message, this);
  commands->parseAnswerData();
  return commands;
}

#endif //__MIO_SIMULATE__

Device::~Device() {
  if (midiin) {
    if (midiin->isPortOpen())
      midiin->closePort();
    delete midiin;
  }
  if (midiout) {
    if (midiout->isPortOpen())
      midiout->closePort();
    delete midiout;
  }
}

BYTE_VECTOR *Device::getManufacturerHeader() {
  if (Device::manufacturerHeader == 0) {
    manufacturerHeader = new BYTE_VECTOR();
    manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[0]);
    manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[1]);
    manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[2]);
  }
  return manufacturerHeader;
}

BYTE_VECTOR *Device::getDeviceHeader() {
  if (deviceHeader == 0) {
    deviceHeader = new BYTE_VECTOR();
    deviceHeader->reserve(productId->getByteValue()->size() +
                          serialNumber->getByteValue()->size());
    deviceHeader->insert(deviceHeader->end(),
                         productId->getByteValue()->begin(),
                         productId->getByteValue()->end());
    deviceHeader->insert(deviceHeader->end(),
                         serialNumber->getByteValue()->begin(),
                         serialNumber->getByteValue()->end());
  }
  return deviceHeader;
}

BYTE_VECTOR *Device::getFullHeader() {
  if (fullHeader == 0) {
    fullHeader = new BYTE_VECTOR();
    fullHeader->reserve(Device::getManufacturerHeader()->size() +
                        getDeviceHeader()->size() + 1);
    fullHeader->insert(fullHeader->end(),
                       Device::getManufacturerHeader()->begin(),
                       Device::getManufacturerHeader()->end());
    fullHeader->push_back(Device::MESSAGE_CLASS);
    fullHeader->insert(fullHeader->end(), getDeviceHeader()->begin(),
                       getDeviceHeader()->end());
  }
  return fullHeader;
}

void Device::setupMidi() {
  std::stringstream name;
  name << "MioConfig In " << serialNumber->getLongValue();
  midiin = MIDI::createMidiIn(name.str());
  midiin->openPort(inPortNumber);
  name << "MioConfig Out " << serialNumber->getLongValue();
  midiout = MIDI::createMidiOut(name.str());
  midiout->openPort(outPortNumber);
}

void Device::sentSysex(BYTE_VECTOR *data) {
  usleep(sysexWaitTime);
  midiout->sendMessage(data);
}

BYTE_VECTOR *Device::retrieveSysex() {
  usleep(sysexWaitTime);
  BYTE_VECTOR *data = new BYTE_VECTOR();
  midiin->getMessage(data);
  if (checkSysex(data))
    return data;
  return 0;
}

bool Device::checkSysex(BYTE_VECTOR *data) {
  BYTE_VECTOR *dataHeader =
      new BYTE_VECTOR(data->begin() + 1, data->begin() + 12);
  BYTE_VECTOR *localHeader = getFullHeader();
  return MIDI::compareByteVector(dataHeader, localHeader);
}

void Device::queryDeviceInfo() {
  GetCommands *c = new GetCommands(this);
  c->setDebug(true);
#ifdef __MIO_SIMULATE__
  commands = simulateCommands(commands);
#else
  commands = (Commands *)c->query();
#endif
  if (commands == 0) {
    std::cerr << "can not query supported commands";
    return;
  }

#if __MIO_SIMULATE__
  if (!deviceIsSimulated) {
#endif //__MIO_SIMULATE
    if (commands->isCommandSupported(SysExMessage::GET_INFO_LIST)) {
      Infos *i = new Infos(this);
      i->execute();
      Infos *ia = (Infos *)i->getAnswer();
    }

    DeviceInfo *di = new DeviceInfo(this);
    di->execute();
    DeviceInfo *dia = (DeviceInfo *)di->getAnswer();
    deviceName = dia->getDataAsString();

    di->setInfoItem(DeviceInfo::ACESSORY_NAME);
    di->execute();
    dia = (DeviceInfo *)di->getAnswer();
    modelName = dia->getDataAsString();

    di->setInfoItem(DeviceInfo::SERIAL_NUMBER);
    di->execute();
    dia = (DeviceInfo *)di->getAnswer();
    serialNumberString = dia->getDataAsString();
#ifdef __MIO_SIMULATE__
  }
#endif //__MIO_SIMULATE__
}

BYTE_VECTOR *Device::nextTransactionId() {
  if (transactionId > 16000)
    transactionId = 0;
  BYTE_VECTOR *v = MIDI::byteSplit(++transactionId, 2);
  return v;
}

BYTE_VECTOR *Device::manufacturerHeader = 0;
