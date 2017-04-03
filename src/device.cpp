#include "device.h"
#include "sysex/commands.h"
#include "sysex/deviceinfo.h"
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
  this->serialNumber = new MIDISysexValue(serialNumber);
  this->productId = new MIDISysexValue(productId);
  this->modelName = modelName;
  this->deviceName = deviceName;
}
#endif //__MIO_DEBUG__

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
  Commands *c = new Commands(this);
  BYTE_VECTOR *query = c->getMIDISysExMessage();
  MIDI::printMessage(query);
  sentSysex(query);
  BYTE_VECTOR *answer = retrieveSysex();
  c->parseAnswer(answer);
  MIDI::printMessage(answer);
  std::cout << "next\n";
  query->clear();

  Infos *i = new Infos(this);
  query = i->getMIDISysExMessage();
  MIDI::printMessage(query);
  sentSysex(query);
  answer = retrieveSysex();
  i->parseAnswer(answer);
  MIDI::printMessage(answer);
  std::cout << "next\n";
  query->clear();

  DeviceInfo *di = new DeviceInfo(this);
  query = di->getMIDISysExMessage();
  MIDI::printMessage(query);
  sentSysex(query);
  answer = retrieveSysex();
  di->parseAnswer(answer);
  MIDI::printMessage(answer);
  std::cout << "next\n";
  query->clear();

  di->setInfoItem(DeviceInfo::ACESSORY_NAME);
  query = di->getMIDISysExMessage();
  MIDI::printMessage(query);
  sentSysex(query);
  answer = retrieveSysex();
  di->parseAnswer(answer);
  MIDI::printMessage(answer);
  std::cout << "next\n";
  query->clear();

  di->setInfoItem(DeviceInfo::SERIAL_NUMBER);
  query = di->getMIDISysExMessage();
  MIDI::printMessage(query);
  sentSysex(query);
  answer = retrieveSysex();
  di->parseAnswer(answer);
  MIDI::printMessage(answer);
  std::cout << "next\n";
  query->clear();
}

BYTE_VECTOR *Device::nextTransactionId() {
  if (transactionId > 16000)
    transactionId = 0;
  BYTE_VECTOR *v = MIDI::byteSplit(++transactionId, 2);
  return v;
}

BYTE_VECTOR *Device::manufacturerHeader = 0;
