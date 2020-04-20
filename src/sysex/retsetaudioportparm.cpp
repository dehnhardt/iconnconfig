#include "retsetaudioportparm.h"
#include "retsetaudioglobalparm.h"

RetSetAudioPortParm::RetSetAudioPortParm(Device *device)
	: PortSysExMessage(RET_SET_AUDIO_PORT_PARM, QUERY, device) {}

RetSetAudioPortParm::~RetSetAudioPortParm() {
	if (m_pAudioPortConfigurations != nullptr) {
		if (m_iNumberOfPortConfigurationBlocks > 0) {
			for (int i = 0; i < m_iNumberOfPortConfigurationBlocks; i++)
				delete m_pAudioPortConfigurations[i];
		}
		delete[] m_pAudioPortConfigurations;
	}
}

void RetSetAudioPortParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_audioPortType = static_cast<pk::AudioPortType>(m_pData->at(3));
	m_iOutputChannels = m_pData->at(4);
	m_iInputChannels = m_pData->at(5);
	m_iNumberOfPortConfigurationBlocks = m_pData->at(6);
	m_pAudioPortConfigurations = new AudioPortConfiguration
		*[static_cast<unsigned long>(m_iNumberOfPortConfigurationBlocks)];
	for (int i = 0; i < m_iNumberOfPortConfigurationBlocks; i++) {
		unsigned long offset = static_cast<unsigned long>(7 + i * 6);
		AudioPortConfiguration *audioPortConfiguration =
			new AudioPortConfiguration();
		audioPortConfiguration->audioConfigurationNumber = m_pData->at(offset);
		audioPortConfiguration->maxAudioChannelsSupported =
			m_pData->at(offset + 1);
		audioPortConfiguration->minInputChannelsSupported =
			m_pData->at(offset + 2);
		audioPortConfiguration->maxInputChannelsSupported =
			m_pData->at(offset + 3);
		audioPortConfiguration->minOutputChannelsSupported =
			m_pData->at(offset + 4);
		audioPortConfiguration->maxOutputChannelsSupported =
			m_pData->at(offset + 5);
		m_pAudioPortConfigurations[i] = audioPortConfiguration;
	}
	unsigned long offset =
		static_cast<unsigned long>(7 + m_iNumberOfPortConfigurationBlocks * 6);
	m_iMaxPortNameLength = m_pData->at(offset);
	offset += 1;
	m_iPortNameLength = m_pData->at(offset);
	offset += 1;
	m_sPortName = std::string(m_pData->begin() + static_cast<int>(offset),
							  m_pData->begin() + static_cast<int>(offset) +
								  m_iPortNameLength);
	offset += static_cast<unsigned long>(m_iPortNameLength);
	m_iDeviceSpecficPortNumer = m_pData->at(offset);
	offset += 1;
	unsigned char c;
	switch (m_audioPortType) {
	case pk::AudioPortType::APT_USB_DEVICE:
		c = m_pData->at(offset);
		m_bPortSupportsIOS = (c & 1) != 0;
		m_bPortSupportsPC = (c & 2) != 0;
		m_bPortIOSEnabled = (c & 4) != 0;
		m_bPortPCEnabled = (c & 8) != 0;
		break;
	case pk::AudioPortType::APT_USB_HOST:
	case pk::AudioPortType::APT_ETHERNET:
		c = m_pData->at(offset);
		m_iJackSpecificDeviceNumber = c;
		break;
	default:
		break;
	}
}

std::vector<unsigned char> *RetSetAudioPortParm::getMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *totalNumberOfAudioPorts =
		MIDI::byteSplit7bit(static_cast<unsigned long>(m_iPortId), 2);
	this->m_pCommandData->at(0) = 0x40;
	data->push_back(m_iCommandVersionNumber);
	data->insert(data->end(), totalNumberOfAudioPorts->begin(),
				 totalNumberOfAudioPorts->end());
	data->push_back(m_audioPortType);
	data->push_back(static_cast<unsigned char>(m_iOutputChannels));
	data->push_back(static_cast<unsigned char>(m_iInputChannels));
	data->push_back(0);
	data->push_back(static_cast<unsigned char>(m_iMaxPortNameLength));
	data->push_back(static_cast<unsigned char>(m_sPortName.size()));
	data->insert(data->end(), m_sPortName.begin(), m_sPortName.end());
	data->push_back(static_cast<unsigned char>(m_iDeviceSpecficPortNumer));
	unsigned char c = 0;
	switch (m_audioPortType) {
	case pk::AudioPortType::APT_USB_DEVICE:
		if (m_bPortSupportsIOS)
			c += 1;
		if (m_bPortSupportsPC)
			c += 2;
		if (m_bPortIOSEnabled)
			c += 4;
		if (m_bPortPCEnabled)
			c += 8;
		data->push_back(c);
		break;
	case pk::AudioPortType::APT_USB_HOST:
	case pk::AudioPortType::APT_ETHERNET:
		data->push_back(static_cast<unsigned char>(m_iDeviceSpecficPortNumer));
		break;
	default:
		break;
	}
	delete totalNumberOfAudioPorts;
	return data;
}

std::string
RetSetAudioPortParm::getAudioPortTypeName(pk::AudioPortType audioPortType) {
	switch (audioPortType) {
	case pk::AudioPortType::APT_NONE:
		return "none";
	case pk::AudioPortType::APT_USB_DEVICE:
		return "USB Device";
	case pk::AudioPortType::APT_USB_HOST:
		return "USB Host";
	case pk::AudioPortType::APT_ETHERNET:
		return "Network Device";
	case pk::AudioPortType::APT_ANALOGUE:
		return "Analogue";
	}
	return "none";
}

std::string RetSetAudioPortParm::getAudioPortTypeName() {
	return getAudioPortTypeName(m_audioPortType);
}

bool RetSetAudioPortParm::getPortNameWritable() {
	return m_iPortNameLength > 0;
}

AudioPortConfiguration *RetSetAudioPortParm::getCurrentAudioConfiguration() {
	unsigned int activeAudioConfiguration =
		this->m_pDevice->getAudioGlobalParm()
			->getNumberOfActiveAudioConfiguration();
	if (activeAudioConfiguration <= sizeof(m_pAudioPortConfigurations))
		return m_pAudioPortConfigurations[activeAudioConfiguration - 1];
	return nullptr;
}

std::string RetSetAudioPortParm::getCurrentAudioConfigurationString() {
	return m_pDevice->getAudioGlobalParm()->getAudioConfigurationString();
}

pk::AudioPortType RetSetAudioPortParm::getAudioPortType() const {
	return m_audioPortType;
}

std::string RetSetAudioPortParm::getPortName() const { return m_sPortName; }

void RetSetAudioPortParm::setPortName(const std::string &sPortName) {
	m_iPortNameLength = static_cast<int>(sPortName.length());
	m_sPortName = sPortName;
}

bool RetSetAudioPortParm::getPortSupportsIOS() const {
	return m_bPortSupportsIOS;
}

bool RetSetAudioPortParm::getPortSupportsPC() const {
	return m_bPortSupportsPC;
}

bool RetSetAudioPortParm::getPortIOSEnabled() const {
	return m_bPortIOSEnabled;
}

void RetSetAudioPortParm::setPortIOSEnabled(bool bPortIOSEnabled) {
	m_bPortIOSEnabled = bPortIOSEnabled;
}

bool RetSetAudioPortParm::getPortPCEnabled() const { return m_bPortPCEnabled; }

void RetSetAudioPortParm::setPortPCEnabled(bool bPortPCEnabled) {
	m_bPortPCEnabled = bPortPCEnabled;
}

int RetSetAudioPortParm::getDeviceSpecficPortNumber() const {
	return m_iDeviceSpecficPortNumer;
}

int RetSetAudioPortParm::getJackSpecificDeviceNumber() const {
	return m_iJackSpecificDeviceNumber;
}

int RetSetAudioPortParm::getMaxPortNameLength() const {
	return m_iMaxPortNameLength;
}

unsigned int RetSetAudioPortParm::getInputChannels() const {
	return m_iInputChannels;
}

void RetSetAudioPortParm::setInputChannels(unsigned int iInputChannels) {
	m_iInputChannels = iInputChannels;
}

unsigned int RetSetAudioPortParm::getOutputChannels() const {
	return m_iOutputChannels;
}

void RetSetAudioPortParm::setOutputChannels(unsigned int iOutputChannels) {
	m_iOutputChannels = iOutputChannels;
}

int RetSetAudioPortParm::getNumberOfPortConfigurationBlocks() const {
	return m_iNumberOfPortConfigurationBlocks;
}
