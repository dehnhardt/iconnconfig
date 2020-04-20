#include "retmixerinputcontrol.h"

RetMixerInputControl::RetMixerInputControl(Device *device)
	: PortSysExMessage(RET_MIXER_INPUT_CONTROL, SysExMessage::QUERY, device) {
	// m_vPanCurves = std::vector<pk::PanCurve>();
}

RetMixerInputControl::~RetMixerInputControl() {}

void RetMixerInputControl::parseAnswerData() {
	unsigned long offset = 0;
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = parsePortId();
	parseExistFlags(m_pData->at(3));
	parseEditFlags(m_pData->at(4));
	offset = 5;
	if (m_bHasPanControl) {
		m_iMaxPanValue =
			static_cast<int>(MIDI::byteJoin7bit(m_pData, offset, 3));
		offset += 3;
		m_iNumberOfPanCurves = m_pData->at(offset);
		offset++;
		for (unsigned int i = 1; i <= m_iNumberOfPanCurves; i++) {
			m_vPanCurves.push_back(pk::PanCurve(m_pData->at(offset)));
			offset++;
		}
	}
	if (m_bHasVolumeControl) {
		m_iMinimumVolumeValue = MIDI::bytesToSignedInt(m_pData, offset, 3);
		offset += 3;
		m_iMaximumVolumeValue = MIDI::bytesToSignedInt(m_pData, offset, 3);
		offset += 3;
		m_iVolumeResolution = MIDI::bytesToSignedInt(m_pData, offset, 3);
		offset += 3;
	}
}

void RetMixerInputControl::parseExistFlags(unsigned char exist_flags) {
	m_bHasControls = exist_flags > 0;
	m_bHasPanControl = (exist_flags & 64);
	m_bHasInvertControl = (exist_flags & 32);
	m_bHasStereoLinkControl = (exist_flags & 16);
	m_bHasSoloPFLControl = (exist_flags & 8);
	m_bHasSoloControl = (exist_flags & 4);
	m_bHasMuteControl = (exist_flags & 2);
	m_bHasVolumeControl = (exist_flags & 1);
}

void RetMixerInputControl::parseEditFlags(unsigned char edit_flags) {
	m_bPanControlEditable = edit_flags & 64;
	m_bInvertControlEditable = edit_flags & 32;
	m_bStereoLinkControlEditable = edit_flags & 16;
	m_bSoloPFLControlEditable = edit_flags & 8;
	m_bSoloControlEditable = edit_flags & 4;
	m_bMuteControlEditable = edit_flags & 2;
	m_bVolumeControlEditable = edit_flags & 1;
}

bool RetMixerInputControl::hasControls() const { return m_bHasControls; }

bool RetMixerInputControl::hasVolumeControl() const {
	return m_bHasVolumeControl;
}

bool RetMixerInputControl::hasMuteControl() const { return m_bHasMuteControl; }

bool RetMixerInputControl::hasSoloControl() const { return m_bHasSoloControl; }

bool RetMixerInputControl::hasSoloPFLControl() const {
	return m_bHasSoloPFLControl;
}

bool RetMixerInputControl::hasStereoLinkControl() const {
	return m_bHasStereoLinkControl;
}

bool RetMixerInputControl::hasInvertControl() const {
	return m_bHasInvertControl;
}

bool RetMixerInputControl::hasPanControl() const { return m_bHasPanControl; }

bool RetMixerInputControl::getVolumeControlEditable() const {
	return m_bVolumeControlEditable;
}

bool RetMixerInputControl::getMuteControlEditable() const {
	return m_bMuteControlEditable;
}

bool RetMixerInputControl::getSoloControlEditable() const {
	return m_bSoloControlEditable;
}

bool RetMixerInputControl::getSoloPFLControlEditable() const {
	return m_bSoloPFLControlEditable;
}

bool RetMixerInputControl::getStereoLinkControlEditable() const {
	return m_bStereoLinkControlEditable;
}

bool RetMixerInputControl::getInvertControlEditable() const {
	return m_bInvertControlEditable;
}

bool RetMixerInputControl::getPanControlEditable() const {
	return m_bPanControlEditable;
}

int RetMixerInputControl::getMaxPanValue() const { return m_iMaxPanValue; }

unsigned int RetMixerInputControl::getNumberOfPanCurves() const {
	return m_iNumberOfPanCurves;
}

int RetMixerInputControl::getMinimumVolumeValue() const {
	return m_iMinimumVolumeValue;
}

int RetMixerInputControl::getMaximumVolumeValue() const {
	return m_iMaximumVolumeValue;
}

int RetMixerInputControl::getVolumeResolution() const {
	return m_iVolumeResolution;
}
