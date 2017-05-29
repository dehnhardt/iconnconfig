#include "device.h"
#include "sysex/getcommandlist.h"
#include "sysex/getinfo.h"
#include "sysex/getinfolist.h"
#include "sysex/getmidiinfo.h"
#include "sysex/getmidiportinfo.h"
#include "sysex/midi.h"
#include "sysex/retcommandlist.h"
#include "sysex/retinfolist.h"
#include "sysex/retsetmidiinfo.h"
#include "sysex/retsetmidiportinfo.h"

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

RetCommandList *Device::simulateCommands() {
  BYTE_VECTOR *message = new BYTE_VECTOR({0xF0});
  message->insert(message->end(), getFullHeader()->begin(),
                  getFullHeader()->end());
  message->push_back(0x00);
  message->push_back(0x01);
  message->push_back(0x00);
  message->push_back(SysExMessage::RET_COMMAND_LIST);
  message->push_back(0x00);
  BYTE_VECTOR *allowedCommands = new BYTE_VECTOR();
  allowedCommands->insert(allowedCommands->end(),
                          {SysExMessage::GET_INFO_LIST, SysExMessage::GET_INFO,
                           SysExMessage::RET_SET_INFO,
                           SysExMessage::GET_RESET_LIST,
                           SysExMessage::GET_SAVE_RESTORE_LIST,
                           SysExMessage::GET_ETHERNET_PORT_INFO});
  message->push_back(allowedCommands->size());
  message->insert(message->end(), allowedCommands->begin(),
                  allowedCommands->end());
  RetCommandList *commands =
      new RetCommandList(SysExMessage::RET_COMMAND_LIST, message, this);
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

void Device::sentSysex(BYTE_VECTOR *data) { midiout->sendMessage(data); }

BYTE_VECTOR *Device::retrieveSysex() {
  BYTE_VECTOR *data = new BYTE_VECTOR();
  int i = 0;
  for (i = 0; i < WAIT_LOOPS && data->size() == 0; i++) {
    SLEEP(WAIT_TIME);
    midiin->getMessage(data);
  }
  std::cout << "delay: " << i << std::endl;
  if (checkSysex(data))
    return data;
  return 0;
}

bool Device::checkSysex(BYTE_VECTOR *data) {
  if (!data || data->size() <= 0)
    return false;
  BYTE_VECTOR *dataHeader =
      new BYTE_VECTOR(data->begin() + 1, data->begin() + 12);
  BYTE_VECTOR *localHeader = getFullHeader();
	return MIDI::compareByteVector(dataHeader, localHeader);
}

void Device::requestMidiPortInfos() {
	int midiPorts = getMidiInfo()->getMidiPorts();
	GetMidiPortInfo *info = new GetMidiPortInfo(this);
	for (int i = 1; i <= midiPorts; ++i) {
		info->setPortNumer(i);
		RetSetMidiPortInfo *midiPortInfo = (RetSetMidiPortInfo *)info->query();
		midiPortInfo->printRawData();
	}
}

bool Device::queryDeviceInfo() {
  GetCommandList *c = new GetCommandList(this);
  c->setDebug(true);
#ifdef __MIO_SIMULATE__
  if (deviceIsSimulated)
    commands = simulateCommands();
  else
    commands = (RetCommandList *)c->query();
#else
  commands = (RetCommandList *)c->query();
#endif
  if (commands == 0) {
    std::cerr << "can not query supported commands";
    return false;
  }

#if __MIO_SIMULATE__
  if (!deviceIsSimulated) {
#endif //__MIO_SIMULATE

    if (commands->isCommandSupported(SysExMessage::GET_INFO_LIST)) {
      GetInfoList *i = new GetInfoList(this);
      i->setDebug(true);
      ii = (RetInfoList *)i->query();
    }

    deviceInfo = new GetInfo(this, ii);

    if (ii->isInfoImplemented(GetInfo::DEVICE_NAME))
      deviceName = deviceInfo->getItemValue(GetInfo::DEVICE_NAME);

    if (ii->isInfoImplemented(GetInfo::ACCESSORY_NAME))
      modelName = deviceInfo->getItemValue(GetInfo::ACCESSORY_NAME);

    if (ii->isInfoImplemented(GetInfo::SERIAL_NUMBER))
      serialNumberString = deviceInfo->getItemValue(GetInfo::SERIAL_NUMBER);

    if (ii->isInfoImplemented(GetInfo::FIRMWARE_VERSION))
      firmwareVersion = deviceInfo->getItemValue(GetInfo::FIRMWARE_VERSION);

    if (ii->isInfoImplemented(GetInfo::HARDWARE_VERSION))
      hardwareVersion = deviceInfo->getItemValue(GetInfo::HARDWARE_VERSION);

    if (ii->isInfoImplemented(GetInfo::MANUFACTURER_NAME))
      manufacturerName = deviceInfo->getItemValue(GetInfo::MANUFACTURER_NAME);

    if (ii->isInfoImplemented(GetInfo::MODEL_NUMBER))
      modelNumber = deviceInfo->getItemValue(GetInfo::MODEL_NUMBER);

		if (commands->isCommandSupported(SysExMessage::GET_MIDI_INFO)) {
			GetMidiInfo *getMidiInfo = new GetMidiInfo(this);
			this->midiInfo = (RetSetMidiInfo *)getMidiInfo->query();
		}
		if (commands->isCommandSupported(SysExMessage::GET_MIDI_PORT_INFO) &&
				this->midiInfo != 0) {
			requestMidiPortInfos();
		}

#ifdef __MIO_SIMULATE__
	}
#endif //__MIO_SIMULATE__
	return true;
}

bool Device::hasMidiSupport() { return (getMidiInfo() != 0); }

BYTE_VECTOR *Device::nextTransactionId() {
	if (transactionId > 16000)
		transactionId = 0;
	BYTE_VECTOR *v = MIDI::byteSplit(++transactionId, 2);
	return v;
}

BYTE_VECTOR *Device::manufacturerHeader = 0;
