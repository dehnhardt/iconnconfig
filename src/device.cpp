#include "device.h"
#include "sysex/communicationexception.h"
#include "sysex/getaudiochannelname.h"
#include "sysex/getaudioglobalparm.h"
#include "sysex/getaudioportparm.h"
#include "sysex/getcommandlist.h"
#include "sysex/getdevice.h"
#include "sysex/getethernetportinfo.h"
#include "sysex/getinfo.h"
#include "sysex/getinfolist.h"
#include "sysex/getmidiinfo.h"
#include "sysex/getmidiportinfo.h"
#include "sysex/getmixerinputparm.h"
#include "sysex/getmixeroutputparm.h"
#include "sysex/getmixerportparm.h"
#include "sysex/getsaverestorelist.h"
#include "sysex/midi.h"
#include "sysex/protocolexception.h"
#include "sysex/retcommandlist.h"
#include "sysex/retinfolist.h"
#include "sysex/retsaverestorelist.h"
#include "sysex/retsetaudiochannelname.h"
#include "sysex/retsetaudioportparm.h"
#include "sysex/retsetmidiinfo.h"
#include "sysex/retsetmidiportinfo.h"
#include "sysex/retsetmixerinputparm.h"
#include "sysex/retsetmixeroutputparm.h"
#include "sysex/retsetmixerportparm.h"

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
	m_pInformationTree->clear();
	delete m_pInformationTree;

	delete m_pSaveRestoreList;
	delete m_pFullHeader;
	delete m_pSerialNumber;
	delete m_pProductId;
	delete midiin;
	delete midiout;
	delete m_pDeviceHeader;
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

std::shared_ptr<RetSetAudioGlobalParm> Device::getGlobalAudioParam() const {
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

	m_pDeviceInfo = std::make_shared<GetInfo>(this, m_pRetInfoList);

	if (m_pRetInfoList->isInfoImplemented(GetInfo::DEVICE_NAME)) {
		m_sDeviceName = m_pDeviceInfo->getItemValue(GetInfo::DEVICE_NAME);
		midiin->setPortName("iConnCfg " + m_sDeviceName);
		midiout->setPortName("iConnCfg " + m_sDeviceName);
	}

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
		std::shared_ptr<RetSaveRestoreList> l =
			std::dynamic_pointer_cast<RetSaveRestoreList>(
				getSaveRestoreList->querySmart());
		m_pSaveRestoreList = l->getSaveRestoreList();
		delete getSaveRestoreList;
	}
	// Audio setings
	if (m_pCommands->isCommandSupported(Command::GET_AUDIO_GLOBAL_PARM)) {
		std::unique_ptr<GetAudioGlobalParm> getAudioGlobalParam =
			std::make_unique<GetAudioGlobalParm>(this);
		getAudioGlobalParam->setDebug(true);
		m_pGlobalAudioParam = std::dynamic_pointer_cast<RetSetAudioGlobalParm>(
			getAudioGlobalParam->querySmart());
	}
	if (m_pCommands->isCommandSupported(Command::GET_MIXER_INPUT_CONTROL)) {
	}
	if (m_pCommands->isCommandSupported(Command::GET_MIXER_OUTPUT_CONTROL)) {
	}
	return true;
}

std::shared_ptr<AudioPortStructure> Device::getAudioPortStructure() {
	if (m_pAudioPortParms == nullptr) {
		queryAudioPorts();
	}
	return m_pAudioPortParms;
}

std::vector<std::shared_ptr<RetSetAudioChannelName>>
Device::getInputChannels() {
	if (m_vInputChannels.empty()) {
		for (AudioPortId i = 1; i <= m_iOutPortNumber; i++) {
			AudioChannelNames acn =
				getAudioChannelNames(i, ChannelDirection::CD_INPUT);
			for (auto channel : acn)
				m_vInputChannels.push_back(channel.second);
		}
	}
	return m_vInputChannels;
}

std::vector<std::shared_ptr<RetSetAudioChannelName>>
Device::getOutputChannels() {
	if (m_vOutputChannels.empty()) {
		for (AudioPortId i = 1; i <= m_iOutPortNumber; i++) {
			AudioChannelNames acn =
				getAudioChannelNames(i, ChannelDirection::CD_OUTPUT);
			for (auto channel : acn)
				m_vOutputChannels.push_back(channel.second);
		}
	}
	return m_vOutputChannels;
}

void Device::queryAudioPorts() {
	m_pAudioPortParms = std::make_shared<AudioPortStructure>();
	m_pAudioChannelStructure = std::make_shared<AudioChannelStructure>();
	unsigned int numberOfAudioPorts =
		getGlobalAudioParam()->getNumberOfAudioPorts();
	std::unique_ptr<GetAudioPortParm> getAudioPortParm =
		std::make_unique<GetAudioPortParm>(this);
	getAudioPortParm->setDebug(false);
	for (AudioPortId i = 1; i <= numberOfAudioPorts; i++) {
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPorts = nullptr;

		AudioDirectionChannels adc;
		getAudioPortParm->setPortId(i);
		std::shared_ptr<RetSetAudioPortParm> retSetAudioPortParm =
			std::dynamic_pointer_cast<RetSetAudioPortParm>(
				getAudioPortParm->querySmart());
		pk::AudioPortType audioPortType = retSetAudioPortParm->getAudioPortType();

		AudioChannelNames acnInput =
			queryAudioChannels(i, ChannelDirection::CD_INPUT,
							   retSetAudioPortParm->getInputChannels());
		adc[ChannelDirection::CD_INPUT] = acnInput;
		AudioChannelNames acnOutput =
			queryAudioChannels(i, ChannelDirection::CD_OUTPUT,
							   retSetAudioPortParm->getOutputChannels());
		adc[ChannelDirection::CD_OUTPUT] = acnOutput;
		try {
			audioPorts = m_pAudioPortParms->at(audioPortType);
		} catch (const std::out_of_range __attribute__((unused)) & oor) {
			audioPorts =
				new std::vector<std::shared_ptr<RetSetAudioPortParm>>();
			m_pAudioPortParms->insert(
				std::pair<pk::AudioPortType,
						  std::vector<std::shared_ptr<RetSetAudioPortParm>> *>(
					audioPortType, audioPorts));
		}
		audioPorts->push_back(retSetAudioPortParm);
		m_pAudioChannelStructure->insert(
			std::pair<unsigned int, AudioDirectionChannels>(i, adc));
		m_mAudioPorts[i] = retSetAudioPortParm;
	}
}

AudioChannelNames Device::queryAudioChannels(unsigned int portId,
											 ChannelDirection direction,
											 unsigned int numberOfChannels) {
	AudioChannelNames audioChannelNames;
	std::unique_ptr<GetAudioChannelName> getAudioChannelName =
		std::make_unique<GetAudioChannelName>(this);
	getAudioChannelName->setPortId(portId);
	getAudioChannelName->setChannelDirection(direction);
	for (AudioChannelId i = 1; i <= numberOfChannels; i++) {
		getAudioChannelName->setChannelNumber(i);
		std::shared_ptr<RetSetAudioChannelName> retSetAudioChannelName =
			std::dynamic_pointer_cast<RetSetAudioChannelName>(
				getAudioChannelName->querySmart());
		if (nullptr != retSetAudioChannelName)
			audioChannelNames[i] = retSetAudioChannelName;
	}
	return audioChannelNames;
}

void Device::queryAudioMixerChannels(ChannelDirection channelDirection) {
	std::shared_ptr<RetSetMixerPortParm> mixerPortParm = getMixerPortParm();
	std::map<unsigned int, AudioPortMixerBlock> audioPortMixerBlocks =
		mixerPortParm->getAudioPortMixerBlocks();

	if ((channelDirection == ChannelDirection::CD_INPUT) &&
		!m_pAudioMixerInputChannels) {
		m_pAudioMixerInputChannels =
			std::make_shared<AudioMixerInputChannels>();
		for (auto audioPortMixerBlockPair : audioPortMixerBlocks) {
			AudioPortId audioMixerPortId = audioPortMixerBlockPair.first;
			AudioPortMixerBlock audioPortMixerBlock =
				audioPortMixerBlockPair.second;

			AudioChannelId audioMixerChannelId;

			for (audioMixerChannelId = 1;
				 audioMixerChannelId <= audioPortMixerBlock.numberOfMixerInputs;
				 ++audioMixerChannelId) {
				std::unique_ptr<GetMixerInputParm> getMixerInputParm =
					std::make_unique<GetMixerInputParm>(this);
				getMixerInputParm->setPortId(audioMixerPortId);
				getMixerInputParm->setMixerInputNumber(audioMixerChannelId);
				AudioPortChannelId channelId =
					channelIndex(audioMixerPortId, AudioPortClass::MIXER_PORT,
								 audioMixerChannelId);
				std::shared_ptr<RetSetMixerInputParm> mixerInputParm =
					std::dynamic_pointer_cast<RetSetMixerInputParm>(
						getMixerInputParm->querySmart());
				m_pAudioMixerInputChannels->insert(
					std::pair<AudioPortChannelId,
							  std::shared_ptr<RetSetMixerInputParm>>(
						channelId, mixerInputParm));
			}
		}
	} else if ((channelDirection == ChannelDirection::CD_OUTPUT) &&
			   !m_pAudioMixerOutputChannels) {
		m_pAudioMixerOutputChannels =
			std::make_shared<AudioMixerOutputChannels>();
		for (auto audioPortMixerBlockPair : audioPortMixerBlocks) {
			AudioPortId audioMixerPortId = audioPortMixerBlockPair.first;
			AudioPortMixerBlock audioPortMixerBlock =
				audioPortMixerBlockPair.second;
			AudioChannelId audioMixerChannelId;

			for (audioMixerChannelId = 1;
				 audioMixerChannelId <=
				 audioPortMixerBlock.numberOfMixerOutputs;
				 ++audioMixerChannelId) {

				std::unique_ptr<GetMixerOutputParm> getMixerOutputParm =
					std::make_unique<GetMixerOutputParm>(this);
				getMixerOutputParm->setPortId(audioMixerPortId);
				getMixerOutputParm->setMixerOutputNumber(audioMixerChannelId);
				AudioPortChannelId channelId =
					channelIndex(audioMixerPortId, AudioPortClass::MIXER_PORT,
								 audioMixerChannelId);
				std::shared_ptr<RetSetMixerOutputParm> mixerOutputParm =
					std::dynamic_pointer_cast<RetSetMixerOutputParm>(
						getMixerOutputParm->querySmart());
				m_pAudioMixerOutputChannels->insert(
					std::pair<AudioPortChannelId,
							  std::shared_ptr<RetSetMixerOutputParm>>(
						channelId, mixerOutputParm));
			}
		}
	}
}

std::shared_ptr<RetSetMixerPortParm> Device::getMixerPortParm() {
	if (!m_pRetSetMixerPortParm)
		queryMixerPortParm();
	return m_pRetSetMixerPortParm;
}

std::shared_ptr<AudioMixerOutputChannels>
Device::getAudioMixerOutputChannels(bool refresh) {
	if (!m_pAudioMixerOutputChannels || refresh) {
		if (refresh)
			m_pAudioMixerOutputChannels = nullptr;
		queryAudioMixerChannels(ChannelDirection::CD_OUTPUT);
	}
	return m_pAudioMixerOutputChannels;
}

std::shared_ptr<AudioMixerInputChannels>
Device::getAudioMixerInputChannels(bool refresh) {
	if (!m_pAudioMixerInputChannels || refresh) {
		if (refresh)
			m_pAudioMixerInputChannels = nullptr;
		queryAudioMixerChannels(ChannelDirection::CD_INPUT);
	}
	return m_pAudioMixerInputChannels;
}

void Device::queryMixerPortParm() {
	if (!m_pRetSetMixerPortParm) {
		std::unique_ptr<GetMixerPortParm> getMixerPortParm =
			std::make_unique<GetMixerPortParm>(this);
		m_pRetSetMixerPortParm = std::dynamic_pointer_cast<RetSetMixerPortParm>(
			getMixerPortParm->querySmart());
	}
}

std::map<AudioPortId, std::shared_ptr<RetSetAudioPortParm>>
Device::getAudioPorts() const {
	return m_mAudioPorts;
}

std::shared_ptr<RetSetAudioPortParm>
Device::getAudioPort(AudioPortId audioPortId) {
	return m_mAudioPorts[audioPortId];
}

std::shared_ptr<AudioChannelStructure> Device::getAudioChannelStructure() {
	if (m_pAudioPortParms == nullptr) {
		queryAudioPorts();
	}
	return m_pAudioChannelStructure;
}

AudioDirectionChannels
Device::getAudioDirectionChannels(AudioPortId audioPortId) {
	try {
		return getAudioChannelStructure()->at(audioPortId);
	} catch (std::out_of_range) {
		return AudioDirectionChannels();
	}
}

AudioChannelNames
Device::getAudioChannelNames(AudioPortId audioPortId,
							 ChannelDirection channelDirection) {
	return getAudioDirectionChannels(audioPortId)[channelDirection];
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
