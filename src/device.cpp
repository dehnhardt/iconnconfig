#include "device.h"
#include "sysex/commands.h"
#include "sysex/deviceinfo.h"
#include "sysex/getcommands.h"
#include "sysex/getinfolist.h"
#include "sysex/implementedinfos.h"
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

Commands *Device::simulateCommands() {
  BYTE_VECTOR *message = new BYTE_VECTOR({0xF0});
  message->insert(message->end(), getFullHeader()->begin(),
                  getFullHeader()->end());
  message->push_back(0x00);
  message->push_back(0x01);
  message->push_back(0x00);
  message->push_back(SysExMessage::RET_COMMAND_LIST);
  message->push_back(0x00);
	BYTE_VECTOR *allowedCommands = new BYTE_VECTOR();
	allowedCommands->insert(
			allowedCommands->end(),
			{SysExMessage::GET_INFO_LIST, SysExMessage::GET_DEVICE_INFO,
			 SysExMessage::RET_SET_DEVICE_INFO, SysExMessage::GET_RESET_LIST,
			 SysExMessage::GET_SAVE_RESTORE_LIST,
			 SysExMessage::GET_ETHERNET_PORT_INFO});
	message->push_back(allowedCommands->size());
	message->insert(message->end(), allowedCommands->begin(),
									allowedCommands->end());
	Commands *commands =
			new Commands(SysExMessage::RET_COMMAND_LIST, message, this);
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
	BYTE_VECTOR *dataHeader =
			new BYTE_VECTOR(data->begin() + 1, data->begin() + 12);
	BYTE_VECTOR *localHeader = getFullHeader();
	return MIDI::compareByteVector(dataHeader, localHeader);
}

void Device::queryDeviceInfo() {
	GetCommands *c = new GetCommands(this);
	c->setDebug(true);
#ifdef __MIO_SIMULATE__
	if (deviceIsSimulated)
		commands = simulateCommands();
	else
		commands = (Commands *)c->query();
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
			GetInfoList *i = new GetInfoList(this);
			i->setDebug(true);
			ii = (ImplementedInfos *)i->query();
		}

		deviceInfo = new DeviceInfo(this, ii);

		if (ii->isInfoImplemented(DeviceInfo::DEVICE_NAME))
			deviceName = deviceInfo->getItemValue(DeviceInfo::DEVICE_NAME);

		if (ii->isInfoImplemented(DeviceInfo::ACCESSORY_NAME))
			modelName = deviceInfo->getItemValue(DeviceInfo::ACCESSORY_NAME);

		if (ii->isInfoImplemented(DeviceInfo::SERIAL_NUMBER))
			serialNumberString = deviceInfo->getItemValue(DeviceInfo::SERIAL_NUMBER);

		if (ii->isInfoImplemented(DeviceInfo::FIRMWARE_VERSION))
			firmwareVersion = deviceInfo->getItemValue(DeviceInfo::FIRMWARE_VERSION);

		if (ii->isInfoImplemented(DeviceInfo::HARDWARE_VERSION))
			hardwareVersion = deviceInfo->getItemValue(DeviceInfo::HARDWARE_VERSION);

		if (ii->isInfoImplemented(DeviceInfo::MANUFACTURER_NAME))
			manufacturerName =
					deviceInfo->getItemValue(DeviceInfo::MANUFACTURER_NAME);

		if (ii->isInfoImplemented(DeviceInfo::MODEL_NUMBER))
			modelNumber = deviceInfo->getItemValue(DeviceInfo::MODEL_NUMBER);
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
