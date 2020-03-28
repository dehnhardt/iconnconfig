#include "retsetmixerinputcontrolvalue.h"

RetSetMixerInputControlValue::RetSetMixerInputControlValue(Device *device)
	: PortSysExMessage(Command::RET_SET_MIXER_INPUT_CONTROL_VALUE,
					   SysExMessage::QUERY, device) {}

RetSetMixerInputControlValue::~RetSetMixerInputControlValue() {}

void RetSetMixerInputControlValue::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<unsigned int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_iMixerOutputNumber = m_pData->at(3);
	m_iMixerInputNumber = m_pData->at(4);
	offset = 5;
	parseExistFlags(m_pData->at(offset));
	offset++;
	if (m_bHasVolumeControl) {
		m_iVolume = MIDI::bytesToSignedInt(m_pData, offset, 3);
		offset += 3;
	}
	if (m_bHasMuteControl) {
		m_bMute = m_pData->at(offset) == 1;
		offset++;
	}
	if (m_bHasSoloControl) {
		m_bSolo = m_pData->at(offset) == 1;
		offset++;
	}
	if (m_bHasSoloPFLControl) {
		m_bSoloPFL = m_pData->at(offset) == 1;
		offset++;
	}
	if (m_bHasStereoLinkControl) {
		m_bSteroLink = m_pData->at(offset) == 1;
		offset++;
	}
	if (m_bHasInvertControl) {
		m_bInvert = m_pData->at(offset) == 1;
		offset++;
	}
	if (m_bHasPanControl) {
		m_iPan = MIDI::bytesToSignedInt(m_pData, offset, 3);
		offset += 3;
		m_iPanCurve = static_cast<PanCurve>(m_pData->at(offset));
	}
}

std::vector<unsigned char> *RetSetMixerInputControlValue::getMessageData() {
	BYTE_VECTOR *messageData = new BYTE_VECTOR();
	BYTE_VECTOR *audioPortId = getPortIdBytes();

	this->m_pCommandData->at(0) = 0x40;
	messageData->push_back(m_iCommandVersionNumber);
	messageData->insert(messageData->end(), audioPortId->begin(),
						audioPortId->end());
	messageData->push_back(m_iMixerOutputNumber);
	messageData->push_back(m_iMixerInputNumber);
	unsigned char existFlag = 0;

	BYTE_VECTOR *variableData = new BYTE_VECTOR();
	if (m_bHasVolumeControl) {
		existFlag += 1;
		BYTE_VECTOR *volume = MIDI::byteSplit7bit(
			MIDI::inv2sComplement(static_cast<short>(m_iVolume)), 3);
		variableData->insert(variableData->end(), volume->begin(),
							 volume->end());
		delete volume;
	}
	if (m_bHasMuteControl) {
		existFlag += 2;
		variableData->push_back(m_bMute ? 1 : 0);
	}
	if (m_bHasSoloControl) {
		existFlag += 4;
		variableData->push_back(m_bSolo ? 1 : 0);
	}
	if (m_bHasSoloPFLControl) {
		existFlag += 8;
		variableData->push_back(m_bSoloPFL ? 1 : 0);
	}
	if (m_bHasStereoLinkControl) {
		existFlag += 16;
		variableData->push_back(m_bSteroLink ? 1 : 0);
	}
	if (m_bHasInvertControl) {
		existFlag += 32;
		variableData->push_back(m_bInvert ? 1 : 0);
	}
	if (m_bHasPanControl) {
		existFlag += 64;
		BYTE_VECTOR *pan = MIDI::byteSplit7bit(
			MIDI::inv2sComplement(static_cast<short>(m_iPan)), 3);
		variableData->insert(variableData->end(), pan->begin(), pan->end());
		delete pan;
		variableData->push_back(m_iPanCurve);
	}
	messageData->push_back(existFlag);
	messageData->insert(messageData->end(), variableData->begin(),
						variableData->end());
	delete audioPortId;
	delete variableData;
	return messageData;
}

void RetSetMixerInputControlValue::reset() {
	m_bHasPanControl = false;
	m_bHasInvertControl = false;
	m_bHasStereoLinkControl = false;
	m_bHasSoloPFLControl = false;
	m_bHasSoloControl = false;
	m_bHasMuteControl = false;
	m_bHasVolumeControl = false;
}

void RetSetMixerInputControlValue::parseExistFlags(unsigned char exist_flags) {
	m_bHasPanControl = (exist_flags & 64);
	m_bHasInvertControl = (exist_flags & 32);
	m_bHasStereoLinkControl = (exist_flags & 16);
	m_bHasSoloPFLControl = (exist_flags & 8);
	m_bHasSoloControl = (exist_flags & 4);
	m_bHasMuteControl = (exist_flags & 2);
	m_bHasVolumeControl = (exist_flags & 1);
}

unsigned char RetSetMixerInputControlValue::getMixerInputNumber() const {
	return m_iMixerInputNumber;
}

unsigned char RetSetMixerInputControlValue::getMixerOutputNumber() const {
	return m_iMixerOutputNumber;
}

bool RetSetMixerInputControlValue::getInvert() const { return m_bInvert; }

void RetSetMixerInputControlValue::setInvert(bool bInvert) {
	m_bInvert = bInvert;
	m_bHasInvertControl = true;
}

bool RetSetMixerInputControlValue::getSoloPFL() const { return m_bSoloPFL; }

void RetSetMixerInputControlValue::setSoloPFL(bool bSoloPFL) {
	m_bSoloPFL = bSoloPFL;
	m_bHasSoloPFLControl = true;
}

bool RetSetMixerInputControlValue::hasSoloPFLControl() const {
	return m_bHasSoloPFLControl;
}

void RetSetMixerInputControlValue::setHasSoloPFLControl(
	bool bHasSoloPFLControl) {
	m_bHasSoloPFLControl = bHasSoloPFLControl;
}

bool RetSetMixerInputControlValue::hasInvertControl() const {
	return m_bHasInvertControl;
}

void RetSetMixerInputControlValue::setHasInvertControl(bool bHasInvertControl) {
	m_bHasInvertControl = bHasInvertControl;
}

bool RetSetMixerInputControlValue::hasPanControl() const {
	return m_bHasPanControl;
}

void RetSetMixerInputControlValue::setHasPanControl(bool bHasPanControl) {
	m_bHasPanControl = bHasPanControl;
}

int RetSetMixerInputControlValue::getVolume() const { return m_iVolume; }

void RetSetMixerInputControlValue::setVolume(int iVolume) {
	m_iVolume = iVolume;
	m_bHasVolumeControl = true;
}

int RetSetMixerInputControlValue::getPan() const { return m_iPan; }

void RetSetMixerInputControlValue::setPan(int pan) {
	m_iPan = pan;
	m_bHasPanControl = true;
}

bool RetSetMixerInputControlValue::getMute() const { return m_bMute; }

void RetSetMixerInputControlValue::setMute(bool bMute) {
	m_bMute = bMute;
	m_bHasMuteControl = true;
}

bool RetSetMixerInputControlValue::getSteroLink() const { return m_bSteroLink; }

void RetSetMixerInputControlValue::setSteroLink(bool bSteroLink) {
	m_bSteroLink = bSteroLink;
	m_bHasStereoLinkControl = true;
}

bool RetSetMixerInputControlValue::getSolo() const { return m_bSolo; }

void RetSetMixerInputControlValue::setSolo(bool solo) {
	m_bSolo = solo;
	m_bHasSoloControl = true;
}

bool RetSetMixerInputControlValue::hasVolumeControl() const {
	return m_bHasVolumeControl;
}

void RetSetMixerInputControlValue::setHasVolumeControl(bool bHasVolumeControl) {
	m_bHasVolumeControl = bHasVolumeControl;
}

bool RetSetMixerInputControlValue::hasMuteControl() const {
	return m_bHasMuteControl;
}

void RetSetMixerInputControlValue::setHasMuteControl(bool bHasMuteControl) {
	m_bHasMuteControl = bHasMuteControl;
}

bool RetSetMixerInputControlValue::hasStereoLinkControl() const {
	return m_bHasStereoLinkControl;
}

void RetSetMixerInputControlValue::setHasStereoLinkControl(
	bool bHasStereoLinkControl) {
	m_bHasStereoLinkControl = bHasStereoLinkControl;
}

bool RetSetMixerInputControlValue::hasSoloControl() const {
	return m_bHasSoloControl;
}

void RetSetMixerInputControlValue::setHasSoloControl(bool hasSoloControl) {
	m_bHasSoloControl = hasSoloControl;
}
