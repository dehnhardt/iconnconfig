#include "device.h"
#include "sysex/communicationexception.h"
#include "sysex/getcommandlist.h"
#include "sysex/getdevice.h"
#include "sysex/getinfo.h"
#include "sysex/getinfolist.h"
#include "sysex/getmidiinfo.h"
#include "sysex/getmidiportinfo.h"
#include "sysex/getsaverestorelist.h"
#include "sysex/midi.h"
#include "sysex/protocolexception.h"
#include "sysex/retcommandlist.h"
#include "sysex/retinfolist.h"
#include "sysex/retsaverestorelist.h"
#include "sysex/retsetmidiinfo.h"
#include "sysex/retsetmidiportinfo.h"

#include <array>
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

Device::Device(unsigned int inPortNumber, unsigned int outPortNumber,
			   unsigned long serialNumber, unsigned int productId) {
	this->inPortNumber = inPortNumber;
	this->outPortNumber = outPortNumber;
	this->serialNumber = new MIDISysexValue(static_cast<long>(serialNumber), 5);
	this->productId = new MIDISysexValue(productId, 2);
	this->debug = true;
	connect();
}

Device::Device(Device *device)
	: Device(
		  device->getInPortNumer(), device->getOutPortNumer(),
		  static_cast<unsigned long>(device->getSerialNumber()->getLongValue()),
		  static_cast<unsigned int>(device->getProductId()->getLongValue())) {}

Device::~Device() { disconnect(); }

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

bool Device::setupMidi() {
	std::cout << "connect" << std::endl;
	std::stringstream nameIn;
	std::stringstream nameOut;
	if (!midiin) {
		nameIn << "MioConfig In " << serialNumber->getLongValue();
		midiin = MIDI::createMidiIn(nameIn.str());
		if (midiin)
			midiin->setErrorCallback(&midiinErrorCallback, this);
		else
			return false;
	}
	if (!midiin->isPortOpen())
		midiin->openPort(inPortNumber);
	if (!midiout) {
		nameOut << "MioConfig Out " << serialNumber->getLongValue();
		midiout = MIDI::createMidiOut(nameOut.str());
		if (midiout)
			midiout->setErrorCallback(&midiOutErrorCallback, this);
		else
			return false;
	}
	if (!midiout->isPortOpen())
		midiout->openPort(outPortNumber);
	return midiin->isPortOpen() && midiout->isPortOpen();
}

void Device::sentSysex(BYTE_VECTOR *data) { midiout->sendMessage(data); }

void Device::disconnect() {
	std::cout << "disconnect" << std::endl;
	if (midiin) {
		if (midiin->isPortOpen())
			midiin->closePort();
		delete midiin;
		midiin = 0;
	}
	if (midiout) {
		if (midiout->isPortOpen())
			midiout->closePort();
		delete midiout;
		midiout = 0;
	}
}

void Device::connect() {
	bool deviceOpen = false;
	for (int i = 0; i < WAIT_LOOPS && !deviceOpen; i++) {
		SLEEP(WAIT_TIME);
		deviceOpen = setupMidi();
	}
}

BYTE_VECTOR *Device::retrieveSysex() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	int i = 0;
	for (i = 0; i < WAIT_LOOPS && data->size() == 0; ++i) {
		SLEEP(WAIT_TIME);
		int y = 0;
		midiin->getMessage(data);
		// if there are other messages, skip them
		while ((data->size() > 0) && (data->at(0) != 0xf0) && (y < 100)) {
			midiin->getMessage(data);
			if (debug)
				std::cout << "Skipping " << std::dec << y << " midi messages"
						  << std::endl;
			y++;
		}
	}
	std::cout << "delay: " << i << std::endl;
	if (checkSysex(data))
		return data;
	else
		throw CommunicationException(CommunicationException::ANSWER_TIMEOOUT);
}

bool Device::checkSysex(BYTE_VECTOR *data) {
	if (!data || data->size() <= 0)
		return false;
	if (data->size() < 20)
		throw new ProtocolException(ProtocolException::MESSAGE_TO_SHORT);
	BYTE_VECTOR *dataHeader =
		new BYTE_VECTOR(data->begin() + 1, data->begin() + 12);
	BYTE_VECTOR *localHeader = getFullHeader();
	return MIDI::compareByteVector(dataHeader, localHeader);
}

void Device::requestMidiPortInfos() {
	int midiPorts = getMidiInfo()->getMidiPorts();
	if (midiPortInfos == 0) {
		midiPortInfos =
			new std::map<int, std::vector<RetSetMidiPortInfo *> *>();
	}
	GetMidiPortInfo *info = new GetMidiPortInfo(this);
	for (int i = 1; i <= midiPorts; ++i) {
		std::vector<RetSetMidiPortInfo *> *v = 0;
		info->setPortNumer(i);
		RetSetMidiPortInfo *midiPortInfo =
			dynamic_cast<RetSetMidiPortInfo *>(info->query());
		int portType = static_cast<int>(midiPortInfo->getPortType());
		portType <<= 8;
		portType += midiPortInfo->getJackNumberOfType();
		try {
			v = midiPortInfos->at(portType);
		} catch (const std::out_of_range &oor) {
			v = new std::vector<RetSetMidiPortInfo *>();
			midiPortInfos->insert(
				std::pair<int, std::vector<RetSetMidiPortInfo *> *>(portType,
																	v));
		}

		if (v == 0) {
			v = new std::vector<RetSetMidiPortInfo *>();
			midiPortInfos->insert(
				std::pair<int, std::vector<RetSetMidiPortInfo *> *>(portType,
																	v));
		}
		v->push_back(midiPortInfo);
	}
}

bool Device::queryDeviceInfo() {
	GetCommandList *c = new GetCommandList(this);
	c->setDebug(true);
	commands = dynamic_cast<RetCommandList *>(c->query());
	if (commands == 0) {
		std::cerr << "can not query supported commands";
		return false;
	}

	if (commands->isCommandSupported(SysExMessage::GET_INFO_LIST)) {
		GetInfoList *i = new GetInfoList(this);
		i->setDebug(true);
		ii = dynamic_cast<RetInfoList *>(i->query());
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
		this->midiInfo = dynamic_cast<RetSetMidiInfo *>(getMidiInfo->query());
	}
	if (commands->isCommandSupported(SysExMessage::GET_MIDI_PORT_INFO) &&
		this->midiInfo != 0) {
		requestMidiPortInfos();
	}
	if (commands->isCommandSupported(SysExMessage::GET_SAVE_RESTORE_LIST)) {
		GetSaveRestoreList *getSaveRestoreList = new GetSaveRestoreList(this);
		RetSaveRestoreList *l =
			dynamic_cast<RetSaveRestoreList *>(getSaveRestoreList->query());
		saveRestoreList = l->getSaveRestoreList();
	}

	return true;
}

bool Device::isDeviceValid() {
	GetDevice *getDevice = new GetDevice(this);
	getDevice->setDebug(true);
	int ret = -3;
	for (int i = 0; i < WAIT_LOOPS; ++i) {
		ret = getDevice->execute();
		if (ret == 0) {
			std::cout << "device is up and answers" << std::endl;
			return true;
		}
		SLEEP(1000);
	}
	return false;
}

MIDI_PORT_INFOS *Device::getMidiPortInfos() const { return midiPortInfos; }

void Device::setDeviceInformation(std::string modelName,
								  std::string deviceName) {
	this->modelName = modelName;
	this->deviceName = deviceName;
}

bool Device::getDebug() const { return debug; }

void Device::setDebug(bool value) { debug = value; }

bool Device::hasMidiSupport() { return (getMidiInfo() != 0); }

BYTE_VECTOR *Device::nextTransactionId() {
	if (transactionId > 16000)
		transactionId = 0;
	BYTE_VECTOR *v = MIDI::byteSplit(++transactionId, 2);
	return v;
}

void midiOutErrorCallback(RtMidiError::Type type, const std::string &errorText,
						  void *userData __attribute__((unused))) {
	std::cout << "UEC (" << static_cast<int>(type) << "): " << errorText
			  << std::endl;
}

void midiinErrorCallback(RtMidiError::Type type __attribute__((unused)),
						 const std::string &errorText,
						 void *userData __attribute__((unused))) {
	std::cout << "IEC " << errorText << std::endl;
}

BYTE_VECTOR *Device::manufacturerHeader = 0;
