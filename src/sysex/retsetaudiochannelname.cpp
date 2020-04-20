#include "retsetaudiochannelname.h"

RetSetAudioChannelName::RetSetAudioChannelName(Device *device)
	: PortSysExMessage(GET_AUDIO_PORT_METER_VALUE, SysExMessage::QUERY,
					   device) {}

RetSetAudioChannelName::~RetSetAudioChannelName() {}

void RetSetAudioChannelName::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = parsePortId();
	m_iAudioChannelNumber = m_pData->at(3);
	m_ChannelDirection = static_cast<pk::ChannelDirection>(m_pData->at(4));
	m_iMaximumChannelNameLength = m_pData->at(5);
	m_iNameLength = m_pData->at(6);
	m_sChannelName =
		std::string(m_pData->begin() + static_cast<long>(7),
					m_pData->begin() + static_cast<long>(7) + m_iNameLength);
}

std::string RetSetAudioChannelName::getChannelName() const {
	return m_sChannelName;
}

void RetSetAudioChannelName::setChannelName(const std::string &sChannelName) {
	m_sChannelName = sChannelName;
}

unsigned int RetSetAudioChannelName::getNameLength() const {
	return m_iNameLength;
}

void RetSetAudioChannelName::setNameLength(unsigned int iNameLength) {
	m_iNameLength = iNameLength;
}

void RetSetAudioChannelName::setAudioChannelNumber(
	unsigned int iAudioChannelNumber) {
	m_iAudioChannelNumber = iAudioChannelNumber;
}

void RetSetAudioChannelName::setChannelDirection(
	const pk::ChannelDirection &channelDirection) {
	m_ChannelDirection = channelDirection;
}
