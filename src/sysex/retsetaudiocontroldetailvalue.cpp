#include "retsetaudiocontroldetailvalue.h"

RetSetAudioControlDetailValue::RetSetAudioControlDetailValue(Device *device)
    : PortSysExMessage(Command::RET_SET_AUDIO_CONTROL_DETAIL_VALUE,
                       SysExMessage::QUERY, device) {}

RetSetAudioControlDetailValue::~RetSetAudioControlDetailValue() {}

void RetSetAudioControlDetailValue::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iControllerNumber = m_pData->at(3);
	m_iDetailNumber = m_pData->at(4);
	m_controllerType = static_cast<pk::AudioControllerType>(m_pData->at(5));
	offset = 6;
	switch (m_controllerType) {
	case pk::AudioControllerType::CT_SELECTOR:
		break;
	case pk::AudioControllerType::CT_FEATUERE:
		parseExistFlags(m_pData->at(offset));
		offset++;
		if (m_bHasVolumeControl) {
			m_iVolume = MIDI::bytesToSignedInt(m_pData, offset, 3);
			offset += 3;
		}
		if (m_bHasVolumeControl) {
			m_iTrim = MIDI::bytesToSignedInt(m_pData, offset, 3);
			offset += 3;
		}
		if (m_bHasMuteControl) {
			m_bMute = m_pData->at(offset) == 1;
			offset++;
		}
		if (m_bHasPhantomPowerControl) {
			m_bPhantomPower = m_pData->at(offset) == 1;
			offset++;
		}
		if (m_bHasHighImpendanceControl) {
			m_bHigImpedance = m_pData->at(offset) == 1;
			offset++;
		}
		if (m_bHasStereoLinkControl) {
			m_bSteroLink = m_pData->at(offset) == 1;
			offset++;
		}
		break;
	case pk::AudioControllerType::CT_CLOCK_SOURCE:
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

std::vector<unsigned char> *RetSetAudioControlDetailValue::getMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	BYTE_VECTOR *audioPortId = getPortIdBytes();

	this->m_pCommandData->at(0) = 0x40;
	messageData->push_back(m_iCommandVersionNumber);
	messageData->insert(messageData->end(), audioPortId->begin(),
	                    audioPortId->end());
	messageData->push_back(m_iControllerNumber);
	messageData->push_back(m_iDetailNumber);
	messageData->push_back(static_cast<unsigned char>(m_controllerType));
	unsigned char existFlag = 0;

	BYTE_VECTOR *variableData = new BYTE_VECTOR();
	if (m_bHasVolumeControl) {
		existFlag += 1;
		BYTE_VECTOR *volume = MIDI::byteSplit7bit(
		    MIDI::inv2sComplement(static_cast<short>(m_iVolume)), 3);
		variableData->insert(variableData->end(), volume->begin(),
		                     volume->end());
		BYTE_VECTOR *trim = MIDI::byteSplit7bit(
		    MIDI::inv2sComplement(static_cast<short>(m_iTrim)), 3);
		variableData->insert(variableData->end(), trim->begin(), trim->end());
		delete trim;
		delete volume;
	}
	if (m_bHasMuteControl) {
		existFlag += 2;
		variableData->push_back(m_bMute ? 1 : 0);
	}
	if (m_bHasPhantomPowerControl) {
		existFlag += 4;
		variableData->push_back(m_bPhantomPower ? 1 : 0);
	}
	if (m_bHasHighImpendanceControl) {
		existFlag += 8;
		variableData->push_back(m_bHigImpedance ? 1 : 0);
	}
	if (m_bHasStereoLinkControl) {
		existFlag += 16;
		variableData->push_back(m_bSteroLink ? 1 : 0);
	}
	messageData->push_back(existFlag);
	messageData->insert(messageData->end(), variableData->begin(),
	                    variableData->end());
	delete audioPortId;
	delete variableData;
	return messageData;
}

void RetSetAudioControlDetailValue::reset() {
	m_bHasMuteControl = false;
	m_bHasVolumeControl = false;
	m_bHasStereoLinkControl = false;
	m_bHasPhantomPowerControl = false;
	m_bHasHighImpendanceControl = false;
}

void RetSetAudioControlDetailValue::parseExistFlags(unsigned char exist_flags) {
	m_bHasStereoLinkControl = (exist_flags & 16);
	m_bHasHighImpendanceControl = (exist_flags & 8);
	m_bHasPhantomPowerControl = (exist_flags & 4);
	m_bHasMuteControl = (exist_flags & 2);
	m_bHasVolumeControl = (exist_flags & 1);
}

int RetSetAudioControlDetailValue::getTrim() const { return m_iTrim; }

void RetSetAudioControlDetailValue::setTrim(int iTrim) {
	m_iTrim = iTrim;
	m_bHasVolumeControl = true;
}

int RetSetAudioControlDetailValue::getVolume() const { return m_iVolume; }

void RetSetAudioControlDetailValue::setVolume(int iVolume) {
	m_iVolume = iVolume;
	m_bHasVolumeControl = true;
}

bool RetSetAudioControlDetailValue::getMute() const { return m_bMute; }

void RetSetAudioControlDetailValue::setMute(bool bMute) {
	m_bMute = bMute;
	m_bHasMuteControl = true;
}

bool RetSetAudioControlDetailValue::getPhantomPower() const {
	return m_bPhantomPower;
}

void RetSetAudioControlDetailValue::setPhantomPower(bool bPhantomPower) {
	m_bPhantomPower = bPhantomPower;
	m_bHasPhantomPowerControl = true;
}

bool RetSetAudioControlDetailValue::getHigImpedance() const {
	return m_bHigImpedance;
}

void RetSetAudioControlDetailValue::setHigImpedance(bool bHigImpedance) {
	m_bHigImpedance = bHigImpedance;
	m_bHasHighImpendanceControl = true;
}

bool RetSetAudioControlDetailValue::getSteroLink() const {
	return m_bSteroLink;
}

void RetSetAudioControlDetailValue::setSteroLink(bool bSteroLink) {
	m_bSteroLink = bSteroLink;
	m_bHasStereoLinkControl = true;
}

bool RetSetAudioControlDetailValue::hasVolumeControl() const {
	return m_bHasVolumeControl;
}

void RetSetAudioControlDetailValue::setHasVolumeControl(
    bool bHasVolumeControl) {
	m_bHasVolumeControl = bHasVolumeControl;
}

bool RetSetAudioControlDetailValue::hasMuteControl() const {
	return m_bHasMuteControl;
}

void RetSetAudioControlDetailValue::setHasMuteControl(bool bHasMuteControl) {
	m_bHasMuteControl = bHasMuteControl;
}

bool RetSetAudioControlDetailValue::hasPhantomPowerControl() const {
	return m_bHasPhantomPowerControl;
}

void RetSetAudioControlDetailValue::setHasPhantomPowerControl(
    bool bHasPhantomPowerControl) {
	m_bHasPhantomPowerControl = bHasPhantomPowerControl;
}

bool RetSetAudioControlDetailValue::hasHighImpendanceControl() const {
	return m_bHasHighImpendanceControl;
}

void RetSetAudioControlDetailValue::setHasHighImpendanceControl(
    bool bHasHighImpendanceControl) {
	m_bHasHighImpendanceControl = bHasHighImpendanceControl;
}

bool RetSetAudioControlDetailValue::hasStereoLinkControl() const {
	return m_bHasStereoLinkControl;
}

void RetSetAudioControlDetailValue::setHasStereoLinkControl(
    bool bHasStereoLinkControl) {
	m_bHasStereoLinkControl = bHasStereoLinkControl;
}
