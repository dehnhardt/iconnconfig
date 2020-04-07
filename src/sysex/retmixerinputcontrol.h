#ifndef RETMIXERINPUTCONTROL_H
#define RETMIXERINPUTCONTROL_H

#include "portsysexmessage.h"

class RetMixerInputControl : public PortSysExMessage {
  public:
	RetMixerInputControl(Device *device);
	RetMixerInputControl(Command cmd, BYTE_VECTOR *message, Device *device)
	    : PortSysExMessage(cmd, message, device) {}
	virtual ~RetMixerInputControl() override;
	void parseAnswerData() override;

	bool hasControls() const;
	bool hasVolumeControl() const;
	bool hasMuteControl() const;
	bool hasSoloControl() const;
	bool hasSoloPFLControl() const;
	bool hasStereoLinkControl() const;
	bool hasInvertControl() const;
	bool hasPanControl() const;

	bool getVolumeControlEditable() const;
	bool getMuteControlEditable() const;
	bool getSoloControlEditable() const;
	bool getSoloPFLControlEditable() const;
	bool getStereoLinkControlEditable() const;
	bool getInvertControlEditable() const;
	bool getPanControlEditable() const;

	int getMaxPanValue() const;
	unsigned int getNumberOfPanCurves() const;
	int getMinimumVolumeValue() const;
	int getMaximumVolumeValue() const;
	int getVolumeResolution() const;

  private:
	bool m_bHasControls = false;
	bool m_bHasVolumeControl = false;
	bool m_bHasMuteControl = false;
	bool m_bHasSoloControl = false;
	bool m_bHasSoloPFLControl = false;
	bool m_bHasStereoLinkControl = false;
	bool m_bHasInvertControl = false;
	bool m_bHasPanControl = false;

	bool m_bVolumeControlEditable = false;
	bool m_bMuteControlEditable = false;
	bool m_bSoloControlEditable = false;
	bool m_bSoloPFLControlEditable = false;
	bool m_bStereoLinkControlEditable = false;
	bool m_bInvertControlEditable = false;
	bool m_bPanControlEditable = false;

	int m_iMaxPanValue = 0;
	unsigned int m_iNumberOfPanCurves = 0;
	std::vector<PanCurve> m_vPanCurves;
	int m_iMinimumVolumeValue = 0;
	int m_iMaximumVolumeValue = 0;
	int m_iVolumeResolution = 0;

  private: // methods
	void parseExistFlags(unsigned char exist_flags);
	void parseEditFlags(unsigned char edit_flags);
};

#endif // RETMIXERINPUTCONTROL_H
