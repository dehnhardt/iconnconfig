#include "retsetaudiocontroldetail.h"

RetSetAudioControlDetail::RetSetAudioControlDetail(Device *device)
	: PortSysExMessage(Command::RET_SET_AUDIO_CONTROL_DETAIL,
					   SysExMessage::QUERY, device) {}

RetSetAudioControlDetail::~RetSetAudioControlDetail() {}

void RetSetAudioControlDetail::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iControllerNumber = m_pData->at(3);
	m_iDetailNumber = m_pData->at(4);
	m_controllerType = static_cast<AudioControllerType>(m_pData->at(5));
	offset = 6;
	switch (m_controllerType) {
	case CT_SELECTOR:
		m_iNameLenght = m_pData->at(offset);
		offset++;
		m_sSelectInputName = std::string(
			m_pData->begin() + static_cast<long>(offset),
			m_pData->begin() + static_cast<long>(offset) + m_iNameLenght);
		break;
	case CT_FEATUERE:
		m_channelDirection = static_cast<ChannelDirection>(m_pData->at(offset));
		offset++;
		m_iChannnelNumber = m_pData->at(offset);
		offset++;
		parseExistFlags(m_pData->at(offset));
		offset++;
		parseEditFlags(m_pData->at(offset));
		offset = 28;
		m_iNameLenght = m_pData->at(offset);
		offset++;
		m_sChannelName = std::string(
			m_pData->begin() + static_cast<long>(offset),
			m_pData->begin() + static_cast<long>(offset) + m_iNameLenght);
		break;
	case CT_CLOCK_SOURCE:
		m_iNameLenght = m_pData->at(offset);
		offset++;
		m_sClockSourceName = std::string(
			m_pData->begin() + static_cast<long>(offset),
			m_pData->begin() + static_cast<long>(offset) + m_iNameLenght);
		break;

	default:
		break;
	}
}

void RetSetAudioControlDetail::parseExistFlags(unsigned char exist_flags) {
	m_bHasStereoLinkControl = (exist_flags & 16);
	m_bHasHighImpendanceControl = (exist_flags & 8);
	m_bHasPhantomPowerControl = (exist_flags & 4);
	m_bHasMuteControl = (exist_flags & 2);
	m_bHasVolumeControl = (exist_flags & 1);
}

void RetSetAudioControlDetail::parseEditFlags(unsigned char edit_flags) {
	m_bStereoLinkControlEditable = edit_flags & 16;
	m_bHighImpendanceControlEditable = edit_flags & 8;
	m_bPhantomPowerControlEditable = edit_flags & 4;
	m_bMuteControlEditable = edit_flags & 2;
	m_bVolumeControlEditable = edit_flags & 1;
}

bool RetSetAudioControlDetail::hasStereoLinkControl() const {
	return m_bHasStereoLinkControl;
}

bool RetSetAudioControlDetail::hasHighImpendanceControl() const {
	return m_bHasHighImpendanceControl;
}

bool RetSetAudioControlDetail::hasPhantomPowerControl() const {
	return m_bHasPhantomPowerControl;
}

bool RetSetAudioControlDetail::hasMuteControl() const {
	return m_bHasMuteControl;
}

bool RetSetAudioControlDetail::hasVolumeControl() const {
	return m_bHasVolumeControl;
}

bool RetSetAudioControlDetail::getStereoLinkControlEditable() const {
	return m_bStereoLinkControlEditable;
}

bool RetSetAudioControlDetail::getHighImpendanceControlEditable() const {
	return m_bHighImpendanceControlEditable;
}

bool RetSetAudioControlDetail::getPhantomPowerControlEditable() const {
	return m_bPhantomPowerControlEditable;
}

bool RetSetAudioControlDetail::getMuteControlEditable() const {
	return m_bMuteControlEditable;
}

bool RetSetAudioControlDetail::getVolumeControlEditable() const {
	return m_bVolumeControlEditable;
}

std::string RetSetAudioControlDetail::getChannelName() const {
	return m_sChannelName;
}
