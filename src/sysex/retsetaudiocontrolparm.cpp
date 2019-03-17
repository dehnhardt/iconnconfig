#include "retsetaudiocontrolparm.h"

RetSetAudioControlParm::RetSetAudioControlParm(Device *device)
	: PortSysExMessage(Command::RET_SET_AUDIO_CONTROL_PARM, SysExMessage::QUERY,
					   device) {}

void RetSetAudioControlParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iControllerNumber = m_pData->at(3);
	m_iControllerType = static_cast<AudioControllerType>(m_pData->at(4));
	unsigned long offset = 0;
	switch (m_iControllerType) {
	case CT_SELECTOR:
		m_iCurrentSelectorInput = m_pData->at(5);
		m_iNumberOfSelectorInputs = m_pData->at(6);
		offset = 7;
		break;
	case CT_FEATUERE:
		m_iNumberOfFeatuireChannels = m_pData->at(5);
		offset = 6;
		break;
	case CT_CLOCK_SOURCE:
		m_iCurrenClockSourceInput = m_pData->at(5);
		m_iNumberOfClockSourceInputs = m_pData->at(6);
		offset = 7;
		break;
	default:
		break;
	}
	m_iControllerNameLength = m_pData->at(offset);
	offset++;
	m_sControllerName = std::string(
		m_pData->begin() + static_cast<long>(offset),
		m_pData->begin() + static_cast<long>(offset) + m_iControllerNameLength);
}

std::vector<unsigned char> *RetSetAudioControlParm::m_pGetMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	return data;
}

std::string RetSetAudioControlParm::getControllerName() const {
	return m_sControllerName;
}

void RetSetAudioControlParm::setControllerName(
	const std::string &sControllerName) {
	m_sControllerName = sControllerName;
}

AudioControllerType RetSetAudioControlParm::getControllerType() const {
	return m_iControllerType;
}

void RetSetAudioControlParm::setControllerType(
	const AudioControllerType &iControllerType) {
	m_iControllerType = iControllerType;
}

unsigned int RetSetAudioControlParm::getControllerNumber() const {
	return m_iControllerNumber;
}

void RetSetAudioControlParm::setControllerNumber(
	unsigned int iControllerNumber) {
	m_iControllerNumber = iControllerNumber;
}

unsigned int RetSetAudioControlParm::getControllerNameLength() const {
	return m_iControllerNameLength;
}

void RetSetAudioControlParm::setControllerNameLength(
	unsigned int iControllerNameLength) {
	m_iControllerNameLength = iControllerNameLength;
}

unsigned int RetSetAudioControlParm::getCurrentSelectorInput() const {
	return m_iCurrentSelectorInput;
}

void RetSetAudioControlParm::setCurrentSelectorInput(
	unsigned int iCurrentSelectorInput) {
	m_iCurrentSelectorInput = iCurrentSelectorInput;
}

unsigned int RetSetAudioControlParm::getNumberOfSelectorInputs() const {
	return m_iNumberOfSelectorInputs;
}

void RetSetAudioControlParm::setNumberOfSelectorInputs(
	unsigned int iNumberOfSelectorInputs) {
	m_iNumberOfSelectorInputs = iNumberOfSelectorInputs;
}

unsigned int RetSetAudioControlParm::getNumberOfFeatuireChannels() const {
	return m_iNumberOfFeatuireChannels;
}

void RetSetAudioControlParm::setNumberOfFeatuireChannels(
	unsigned int iNumberOfFeatuireChannels) {
	m_iNumberOfFeatuireChannels = iNumberOfFeatuireChannels;
}

unsigned int RetSetAudioControlParm::getCurrenClockSourceInput() const {
	return m_iCurrenClockSourceInput;
}

void RetSetAudioControlParm::setCurrenClockSourceInput(
	unsigned int iCurrenClockSourceInput) {
	m_iCurrenClockSourceInput = iCurrenClockSourceInput;
}

unsigned int RetSetAudioControlParm::getNumberOfClockSourceInputs() const {
	return m_iNumberOfClockSourceInputs;
}

void RetSetAudioControlParm::setNumberOfClockSourceInputs(
	unsigned int iNumberOfClockSourceInputs) {
	m_iNumberOfClockSourceInputs = iNumberOfClockSourceInputs;
}
