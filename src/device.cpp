#include "device.h"
#include "sysex/communicationexception.h"
#include "sysex/getaudioglobalparm.h"
#include "sysex/getcommandlist.h"
#include "sysex/getdevice.h"
#include "sysex/getethernetportinfo.h"
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
	this->m_iInPortNumber = inPortNumber;
	this->m_iOutPortNumber = outPortNumber;
	this->m_pSerialNumber =
	    new MIDISysexValue(static_cast<long>(serialNumber), 5);
	this->m_pProductId = new MIDISysexValue(productId, 2);
	this->debug = true;
	this->m_pInformationTree = new DeviceStructure;
	try {
		connect();
	} catch (CommunicationException *e) {
		throw e;
	}
}

Device::Device(Device *device)
    : Device(
          device->getInPortNumer(), device->getOutPortNumer(),
          static_cast<unsigned long>(device->getSerialNumber()->getLongValue()),
          static_cast<unsigned int>(device->getProductId()->getLongValue())) {}

Device::~Device() {
	disconnect();

	MidiPortInfos::iterator it;
	for (it = m_pMidiPortInfos->begin(); it != m_pMidiPortInfos->end(); it++) {
		std::vector<std::shared_ptr<RetSetMidiPortInfo>> *v = it->second;
		v->clear();
		delete v;
	}
	m_pMidiPortInfos->clear();
	delete m_pMidiPortInfos;
	m_pMidiPortInfos = nullptr;

	delete m_pGlobalAudioParam;

	delete midiin;
	delete midiout;
}

BYTE_VECTOR *Device::getManufacturerHeader() {
	if (Device::manufacturerHeader == nullptr) {
		manufacturerHeader = new BYTE_VECTOR();
		manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[0]);
		manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[1]);
		manufacturerHeader->push_back(MANUFACTURER_SYSEX_ID[2]);
	}
	return manufacturerHeader;
}

BYTE_VECTOR *Device::getDeviceHeader() {
	if (m_pDeviceHeader == nullptr) {
		m_pDeviceHeader = new BYTE_VECTOR();
		m_pDeviceHeader->reserve(m_pProductId->getByteValue()->size() +
		                         m_pSerialNumber->getByteValue()->size());
		m_pDeviceHeader->insert(m_pDeviceHeader->end(),
		                        m_pProductId->getByteValue()->begin(),
		                        m_pProductId->getByteValue()->end());
		m_pDeviceHeader->insert(m_pDeviceHeader->end(),
		                        m_pSerialNumber->getByteValue()->begin(),
		                        m_pSerialNumber->getByteValue()->end());
	}
	return m_pDeviceHeader;
}

BYTE_VECTOR *Device::getFullHeader() {
	if (m_pFullHeader == nullptr) {
		m_pFullHeader = new BYTE_VECTOR();
		m_pFullHeader->reserve(Device::getManufacturerHeader()->size() +
		                       getDeviceHeader()->size() + 1);
		m_pFullHeader->insert(m_pFullHeader->end(),
		                      Device::getManufacturerHeader()->begin(),
		                      Device::getManufacturerHeader()->end());
		m_pFullHeader->push_back(Device::MESSAGE_CLASS);
		m_pFullHeader->insert(m_pFullHeader->end(), getDeviceHeader()->begin(),
		                      getDeviceHeader()->end());
	}
	return m_pFullHeader;
}

bool Device::setupMidi() {
	std::cout << "connect" << std::endl;
	std::stringstream nameIn;
	std::stringstream nameOut;
	if (!midiin) {
		nameIn << "MioConfig In " << m_pSerialNumber->getLongValue();
		midiin = MIDI::createMidiIn(nameIn.str());
		if (!midiin)
			return false;
	}
	if (!midiin->isPortOpen())
		try {
		    midiin->openPort(m_iInPortNumber);
	    } catch (...) {
		    throw;
	    }

	if (!midiout) {
		nameOut << "MioConfig Out " << m_pSerialNumber->getLongValue();
		midiout = MIDI::createMidiOut(nameOut.str());
		if (!midiout)
			return false;
	}
	if (!midiout->isPortOpen())
		try {
		    midiout->openPort(m_iOutPortNumber);
	    } catch (...) {
		    throw;
	    }

	return midiin->isPortOpen() && midiout->isPortOpen();
}

void Device::sentSysex(BYTE_VECTOR *data) {
	setLastSendMessage(data);
	midiout->sendMessage(data);
}

void Device::disconnect() {
	std::cout << "disconnect" << std::endl;
	if (midiin) {
		if (midiin->isPortOpen())
			midiin->closePort();
		delete midiin;
		midiin = nullptr;
	}
	if (midiout) {
		if (midiout->isPortOpen())
			midiout->closePort();
		delete midiout;
		midiout = nullptr;
	}
}

void Device::connect() {
	bool deviceOpen = false;
	for (int i = 0; i < WAIT_LOOPS && !deviceOpen; i++) {
		SLEEP(WAIT_TIME);
		try {
			deviceOpen = setupMidi();
		} catch (RtMidiError e) {
			throw new CommunicationException(e);
		}
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
#ifdef __RTMIDI_DEBUG__
	std::cout << "delay: " << i << std::endl;
#endif
	try {
		checkSysex(data);
	} catch (...) {
		throw;
	}
	setLastRetrieveMessage(data);
	return data;
}

bool Device::checkSysex(BYTE_VECTOR *data) {
	if (!data || data->size() <= 0)
		throw new CommunicationException(
	        CommunicationException::ANSWER_TIMEOOUT, this);
	if (data->size() < 20)
		throw new ProtocolException(ProtocolException::MESSAGE_TO_SHORT, this);
	BYTE_VECTOR *dataHeader =
	    new BYTE_VECTOR(data->begin() + 1, data->begin() + 12);
	BYTE_VECTOR *localHeader = getFullHeader();
	if (!MIDI::compareByteVector(dataHeader, localHeader))
		throw new ProtocolException(ProtocolException::WRONG_HEADER, this);
	delete dataHeader;
	return true;
}

void Device::requestMidiPortInfos() {
	int midiPorts = getMidiInfo()->getMidiPorts();
	if (m_pMidiPortInfos == nullptr) {
		m_pMidiPortInfos =
		    new std::map<int,
		                 std::vector<std::shared_ptr<RetSetMidiPortInfo>> *>();
	}
	GetMidiPortInfo *info = new GetMidiPortInfo(this);
	for (int i = 1; i <= midiPorts; ++i) {
		std::vector<std::shared_ptr<RetSetMidiPortInfo>> *v = nullptr;
		info->setPortNumer(i);
		std::shared_ptr<RetSetMidiPortInfo> midiPortInfo;
		try {
			midiPortInfo = std::dynamic_pointer_cast<RetSetMidiPortInfo>(
			    info->querySmart());
		} catch (CommunicationException *ce) {
			std::cerr << ce->getErrorMessage();
			continue;
		}

		int portType = static_cast<int>(midiPortInfo->getPortType());
		portType <<= 8;
		portType += midiPortInfo->getJackNumberOfType();
		try {
			v = m_pMidiPortInfos->at(portType);
		} catch (const std::out_of_range __attribute__((unused)) & oor) {
			v = new std::vector<std::shared_ptr<RetSetMidiPortInfo>>;
			m_pMidiPortInfos->insert(
			    std::pair<int,
			              std::vector<std::shared_ptr<RetSetMidiPortInfo>> *>(
			        portType, v));
		}

		if (v == nullptr) {
			v = new std::vector<std::shared_ptr<RetSetMidiPortInfo>>();
			m_pMidiPortInfos->insert(
			    std::pair<int,
			              std::vector<std::shared_ptr<RetSetMidiPortInfo>> *>(
			        portType, v));
		}
		v->push_back(midiPortInfo);
	}
	delete info;
}

void Device::addCommandToStructure(
    Command cmd, DeviceStructureContainer *structureContainer) {
	m_pInformationTree->insert(std::pair<Command, DeviceStructureContainer *>(
	    cmd, structureContainer));
}

RetSetAudioGlobalParm *Device::getGlobalAudioParam() const {
	return m_pGlobalAudioParam;
}

bool Device::queryDeviceInfo() {

	std::shared_ptr<GetCommandList> getCommandList =
	    std::make_shared<GetCommandList>(this);
	getCommandList->setDebug(true);
	try {
		m_pCommands = std::dynamic_pointer_cast<RetCommandList>(
		    getCommandList->querySmart());
		if (m_pCommands)
			addCommandToStructure(m_pCommands->getCommand(),
			                      new DeviceStructureContainer(m_pCommands));
	} catch (...) {
		throw;
	}
	// delete getCommandList;

	if (m_pCommands->isCommandSupported(Command::GET_INFO_LIST)) {
		std::shared_ptr<GetInfoList> getInfoList =
		    std::make_shared<GetInfoList>(this);
		getInfoList->setDebug(true);
		try {
			m_pRetInfoList = std::dynamic_pointer_cast<RetInfoList>(
			    getInfoList->querySmart());
			DeviceStructureContainer *c = new DeviceStructureContainer(
			    std::dynamic_pointer_cast<SysExMessage>(m_pRetInfoList));
			addCommandToStructure(m_pRetInfoList->getCommand(), c);
		} catch (...) {
			throw;
		}
		// delete getInfoList;
	}

	m_pDeviceInfo = new GetInfo(this, m_pRetInfoList);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::DEVICE_NAME))
		m_sDeviceName = m_pDeviceInfo->getItemValue(GetInfo::DEVICE_NAME);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::ACCESSORY_NAME))
		m_sModelName = m_pDeviceInfo->getItemValue(GetInfo::ACCESSORY_NAME);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::SERIAL_NUMBER))
		m_sSerialNumber = m_pDeviceInfo->getItemValue(GetInfo::SERIAL_NUMBER);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::FIRMWARE_VERSION))
		m_sFirmwareVersion =
		    m_pDeviceInfo->getItemValue(GetInfo::FIRMWARE_VERSION);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::HARDWARE_VERSION))
		m_sHardwareVersion =
		    m_pDeviceInfo->getItemValue(GetInfo::HARDWARE_VERSION);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::MANUFACTURER_NAME))
		m_sManufacturerName =
		    m_pDeviceInfo->getItemValue(GetInfo::MANUFACTURER_NAME);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::MODEL_NUMBER))
		m_sModelNumber = m_pDeviceInfo->getItemValue(GetInfo::MODEL_NUMBER);

	if (m_pCommands->isCommandSupported(Command::GET_MIDI_INFO)) {
		std::shared_ptr<GetMidiInfo> getMidiInfo =
		    std::make_shared<GetMidiInfo>(this);
		this->m_pMidiInfo = std::dynamic_pointer_cast<RetSetMidiInfo>(
		    getMidiInfo->querySmart());
	}
	if (m_pCommands->isCommandSupported(Command::GET_MIDI_PORT_INFO) &&
	    this->m_pMidiInfo != nullptr)
		requestMidiPortInfos();
	if (m_pCommands->isCommandSupported(Command::GET_SAVE_RESTORE_LIST)) {
		GetSaveRestoreList *getSaveRestoreList = new GetSaveRestoreList(this);
		RetSaveRestoreList *l =
		    dynamic_cast<RetSaveRestoreList *>(getSaveRestoreList->query());
		saveRestoreList = l->getSaveRestoreList();
		delete getSaveRestoreList;
		// delete l;
	}
	/*
	if (m_pCommands->isCommandSupported(Command::GET_AUDIO_GLOBAL_PARM)) {
		GetAudioGlobalParm *getAudioGlobalParam = new GetAudioGlobalParm(this);
		getAudioGlobalParam->setDebug(true);
		m_pGlobalAudioParam =
			dynamic_cast<RetSetAudioGlobalParm *>(getAudioGlobalParam->query());
		// delete getAudioGlobalParam;
	}*/
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

SysExMessage *Device::getSysExMessage(Command cmd) {
	std::shared_ptr<SysExMessage> m;
	switch (cmd) {
	case GET_COMMAND_LIST:
		m = std::make_shared<GetCommandList>(this);
		break;
	case Command::GET_ETHERNET_PORT_INFO:
		m = std::make_shared<GetEthernetPortInfo>(this);
		break;
	case Command::GET_INFO_LIST:
		m = std::make_shared<GetInfoList>(this);
		break;
	case Command::GET_MIDI_INFO:
		m = std::make_shared<GetMidiInfo>(this);
		break;
	default:
		return nullptr;
	}
	addCommandToStructure(cmd, new DeviceStructureContainer(m));
	return m.get();
}

MidiPortInfos *Device::getMidiPortInfos() const { return m_pMidiPortInfos; }

void Device::setDeviceInformation(std::string modelName,
                                  std::string deviceName) {
	this->m_sModelName = modelName;
	this->m_sDeviceName = deviceName;
}

std::vector<unsigned char> *Device::getLastSendMessage() const {
	return m_pLastSendMessage;
}

std::vector<unsigned char> *Device::getLastRetrieveMessage() const {
	return m_pLastRetrieveMessage;
}

void Device::setLastSendMessage(std::vector<unsigned char> *value) {
	m_pLastSendMessage = value;
}

void Device::setLastRetrieveMessage(std::vector<unsigned char> *value) {
	m_pLastRetrieveMessage = value;
}

bool Device::getDebug() const { return debug; }

void Device::setDebug(bool value) { debug = value; }

bool Device::hasMidiSupport() { return (getMidiInfo() != nullptr); }

bool Device::hasAudioSupport() { return m_pGlobalAudioParam != nullptr; }

BYTE_VECTOR *Device::nextTransactionId() {
	if (m_iTransactionId > 16000)
		m_iTransactionId = 0;
	BYTE_VECTOR *v = MIDI::byteSplit7bit(++m_iTransactionId, 2);
	return v;
}

bool Device::loadConfigurationFromDevice() {
	getCommands();

	return true;
}

BYTE_VECTOR *Device::manufacturerHeader = nullptr;
