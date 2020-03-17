#include "retmixeroutputcontrol.h"

RetMixerOutputControl::RetMixerOutputControl(Device *device)
	: PortSysExMessage(RET_MIXER_OUTPUT_CONTROL, SysExMessage::QUERY, device) {
	// m_vPanCurves = std::vector<PanCurve>();
}

RetMixerOutputControl::~RetMixerOutputControl() {}

void RetMixerOutputControl::parseAnswerData() {
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
			m_vPanCurves.push_back(PanCurve(m_pData->at(offset)));
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

void RetMixerOutputControl::parseExistFlags(unsigned char exist_flags) {
	m_bHasControls = exist_flags > 0;
	m_bHasPanControl = (exist_flags & 64);
	m_bHasInvertControl = (exist_flags & 32);
	m_bHasStereoLinkControl = (exist_flags & 16);
	m_bHasSoloPFLControl = (exist_flags & 8);
	m_bHasSoloControl = (exist_flags & 4);
	m_bHasMuteControl = (exist_flags & 2);
	m_bHasVolumeControl = (exist_flags & 1);
}

void RetMixerOutputControl::parseEditFlags(unsigned char edit_flags) {
	m_bPanControlEditable = edit_flags & 64;
	m_bInvertControlEditable = edit_flags & 32;
	m_bStereoLinkControlEditable = edit_flags & 16;
	m_bSoloPFLControlEditable = edit_flags & 8;
	m_bSoloControlEditable = edit_flags & 4;
	m_bMuteControlEditable = edit_flags & 2;
	m_bVolumeControlEditable = edit_flags & 1;
}

bool RetMixerOutputControl::getHasControls() const { return m_bHasControls; }

bool RetMixerOutputControl::getHasVolumeControl() const {
	return m_bHasVolumeControl;
}

bool RetMixerOutputControl::getHasMuteControl() const {
	return m_bHasMuteControl;
}

bool RetMixerOutputControl::getHasSoloControl() const {
	return m_bHasSoloControl;
}

bool RetMixerOutputControl::getHasSoloPFLControl() const {
	return m_bHasSoloPFLControl;
}

bool RetMixerOutputControl::getHasStereoLinkControl() const {
	return m_bHasStereoLinkControl;
}

bool RetMixerOutputControl::getHasInvertControl() const {
	return m_bHasInvertControl;
}

bool RetMixerOutputControl::getHasPanControl() const {
	return m_bHasPanControl;
}

bool RetMixerOutputControl::getVolumeControlEditable() const {
	return m_bVolumeControlEditable;
}

bool RetMixerOutputControl::getMuteControlEditable() const {
	return m_bMuteControlEditable;
}

bool RetMixerOutputControl::getSoloControlEditable() const {
	return m_bSoloControlEditable;
}

bool RetMixerOutputControl::getSoloPFLControlEditable() const {
	return m_bSoloPFLControlEditable;
}

bool RetMixerOutputControl::getStereoLinkControlEditable() const {
	return m_bStereoLinkControlEditable;
}

bool RetMixerOutputControl::getInvertControlEditable() const {
	return m_bInvertControlEditable;
}

bool RetMixerOutputControl::getPanControlEditable() const {
	return m_bPanControlEditable;
}

int RetMixerOutputControl::getMaxPanValue() const { return m_iMaxPanValue; }

unsigned int RetMixerOutputControl::getNumberOfPanCurves() const {
	return m_iNumberOfPanCurves;
}

int RetMixerOutputControl::getMinimumVolumeValue() const {
	return m_iMinimumVolumeValue;
}

int RetMixerOutputControl::getMaximumVolumeValue() const {
	return m_iMaximumVolumeValue;
}

int RetMixerOutputControl::getVolumeResolution() const {
	return m_iVolumeResolution;
}
