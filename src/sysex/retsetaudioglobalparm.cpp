#include "retsetaudioglobalparm.h"
#include <sstream>
#include <string>

RetSetAudioGlobalParm::~RetSetAudioGlobalParm() {
	for (unsigned int i = 0; i < m_iNumberOfConfigBlocks; i++) {
		delete m_pAudioConfigurations[i];
	}
	delete[] m_pAudioConfigurations;
}

void RetSetAudioGlobalParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iNumberOfAudioPorts =
	    static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iMinAudioFramesBuffered = m_pData->at(3);
	m_iMaxAudioFramesBuffered = m_pData->at(4);
	m_iCurrentAudioFramesBuffered = m_pData->at(5);
	m_iMinAllowedSyncFactor = m_pData->at(6);
	m_iMaxAllowedSyncFactor = m_pData->at(7);
	m_iCurrentSyncFactor = m_pData->at(8);
	m_iNumberOfActiveAudioConfiguration = m_pData->at(9);
	m_iNumberOfConfigBlocks = m_pData->at(10);
	m_pAudioConfigurations = new AUDIOConfiguration *[m_iNumberOfConfigBlocks];
	for (unsigned int i = 0;
	     (i < m_iNumberOfConfigBlocks) && (i * 3 + 10 < m_iDataLength); i++) {
		AUDIOConfiguration *audioConfiguration = new AUDIOConfiguration();
		audioConfiguration->configurationNumber = m_pData->at(11 + i * 3);
		audioConfiguration->bitDepthCode = m_pData->at(12 + i * 3);
		audioConfiguration->sampleRateCode = m_pData->at(13 + i * 3);
		m_pAudioConfigurations[i] = audioConfiguration;
	}
}

AUDIOConfiguration *RetSetAudioGlobalParm::getAudioConfiguration(int i) const {
	return m_pAudioConfigurations[i];
}

void RetSetAudioGlobalParm::setAudioConfiguration(
    AUDIOConfiguration *pAudioConfigurations, int i) {
	m_pAudioConfigurations[i] = pAudioConfigurations;
}

unsigned int RetSetAudioGlobalParm::getNumberOfAudioPorts() const {
	return m_iNumberOfAudioPorts;
}

unsigned int RetSetAudioGlobalParm::getNumberOfConfigBlocks() const {
	return m_iNumberOfConfigBlocks;
}

unsigned int
RetSetAudioGlobalParm::getNumberOfActiveAudioConfiguration() const {
	return m_iNumberOfActiveAudioConfiguration;
}

void RetSetAudioGlobalParm::setNumberOfActiveAudioConfiguration(
    unsigned int iNumberOfActiveAudioConfigurations) {
	m_iNumberOfActiveAudioConfiguration = iNumberOfActiveAudioConfigurations;
}

unsigned int RetSetAudioGlobalParm::getCurrentSyncFactor() const {
	return m_iCurrentSyncFactor;
}

void RetSetAudioGlobalParm::setCurrentSyncFactor(
    unsigned int iCurrentSyncFactor) {
	m_iCurrentSyncFactor = iCurrentSyncFactor;
}

unsigned int RetSetAudioGlobalParm::getMaxAllowedSyncFactor() const {
	return m_iMaxAllowedSyncFactor;
}

unsigned int RetSetAudioGlobalParm::getMinAllowedSyncFactor() const {
	return m_iMinAllowedSyncFactor;
}

unsigned int RetSetAudioGlobalParm::getCurrentAudioFramesBuffered() const {
	return m_iCurrentAudioFramesBuffered;
}

void RetSetAudioGlobalParm::setCurrentAudioFramesBuffered(
    unsigned int iCurrentAudioFramesBuffered) {
	m_iCurrentAudioFramesBuffered = iCurrentAudioFramesBuffered;
}

unsigned int RetSetAudioGlobalParm::getMaxAudioFramesBuffered() const {
	return m_iMaxAudioFramesBuffered;
}

unsigned int RetSetAudioGlobalParm::getMinAudioFramesBuffered() const {
	return m_iMinAudioFramesBuffered;
}

std::string RetSetAudioGlobalParm::getAudioConfigurationString(unsigned int i) {
	AUDIOConfiguration *audioConfiguration = m_pAudioConfigurations[i - 1];
	std::stringstream configString;
	configString << tr("Sample Rate: ").toStdString()
	             << translateSampleRateText(audioConfiguration->sampleRateCode)
	             << tr(", Bit Depth: ").toStdString()
	             << translateBitDepth(audioConfiguration->bitDepthCode);
	return configString.str();
}

std::string RetSetAudioGlobalParm::getAudioConfigurationString() {
	return getAudioConfigurationString(m_iNumberOfActiveAudioConfiguration);
}

std::vector<unsigned char> *RetSetAudioGlobalParm::getMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	BYTE_VECTOR *audioPorts = MIDI::byteSplit8bit(m_iNumberOfAudioPorts, 2);
	this->m_pCommandData->at(0) = 0x40;

	messageData->push_back(m_iCommandVersionNumber);
	messageData->insert(messageData->end(), audioPorts->begin(),
	                    audioPorts->end());
	messageData->push_back(
	    static_cast<unsigned char>(m_iMinAudioFramesBuffered));
	messageData->push_back(
	    static_cast<unsigned char>(m_iMaxAudioFramesBuffered));
	messageData->push_back(
	    static_cast<unsigned char>(m_iCurrentAudioFramesBuffered));
	messageData->push_back(static_cast<unsigned char>(m_iMinAllowedSyncFactor));
	messageData->push_back(static_cast<unsigned char>(m_iMaxAllowedSyncFactor));
	messageData->push_back(static_cast<unsigned char>(m_iCurrentSyncFactor));
	messageData->push_back(
	    static_cast<unsigned char>(m_iNumberOfActiveAudioConfiguration));
	delete audioPorts;
	return messageData;
}

unsigned int RetSetAudioGlobalParm::translateBitDepth(int bitCode) {
	switch (bitCode) {
	case 1:
		return 4;
	case 2:
		return 8;
	case 3:
		return 12;
	case 4:
		return 16;
	case 5:
		return 20;
	case 6:
		return 24;
	case 7:
		return 28;
	case 8:
		return 32;
	}
	return 0;
}

std::string RetSetAudioGlobalParm::translateBitDepthText(int bitCode) {
	unsigned int bitDepth = translateBitDepth(bitCode);
	std::stringstream ss;
	ss << bitDepth << " bit";
	return ss.str();
}

unsigned int RetSetAudioGlobalParm::translateSampleRate(int sampleRateCode) {
	switch (sampleRateCode) {
	case 1:
		return 11025;
	case 2:
		return 12000;
	case 3:
		return 22050;
	case 4:
		return 24000;
	case 5:
		return 44100;
	case 6:
		return 48000;
	case 7:
		return 88200;
	case 8:
		return 96000;
	}
	return 0;
}

std::string RetSetAudioGlobalParm::translateSampleRateText(int sampleRateCode) {
	unsigned int sampleRate = translateSampleRate(sampleRateCode);
	std::stringstream ss;
	ss << sampleRate << " hz";
	return ss.str();
}
