#ifndef RETSETAUDIOCONTROLDETAIL_H
#define RETSETAUDIOCONTROLDETAIL_H

#include "portsysexmessage.h"

class RetSetAudioControlDetail : public PortSysExMessage {
  public:
	RetSetAudioControlDetail(Device *m_pDevice);

	RetSetAudioControlDetail(Command cmd, BYTE_VECTOR *message, Device *device)
	    : PortSysExMessage(cmd, message, device) {}

	virtual ~RetSetAudioControlDetail() override;
	void parseAnswerData() override;
	void parseExistFlags(unsigned char exist_flags);
	void parseEditFlags(unsigned char edit_flags);

	bool hasStereoLinkControl() const;
	bool hasHighImpendanceControl() const;
	bool hasPhantomPowerControl() const;
	bool hasMuteControl() const;
	bool hasVolumeControl() const;

	bool getStereoLinkControlEditable() const;
	bool getHighImpendanceControlEditable() const;
	bool getPhantomPowerControlEditable() const;
	bool getMuteControlEditable() const;
	bool getVolumeControlEditable() const;

	std::string getChannelName() const;

  private:
	unsigned int m_iControllerNumber = 0;
	unsigned int m_iDetailNumber = 0;
	AudioControllerType m_controllerType = AudioControllerType::CT_NONE;
	ChannelDirection m_channelDirection = CD_NONE;
	unsigned int m_iChannnelNumber = 0;
	unsigned int m_iNameLenght = 0;
	std::string m_sSelectInputName;
	std::string m_sChannelName;
	std::string m_sClockSourceName;

	// feature
	bool m_bHasStereoLinkControl = false;
	bool m_bHasHighImpendanceControl = false;
	bool m_bHasPhantomPowerControl = false;
	bool m_bHasMuteControl = false;
	bool m_bHasVolumeControl = false;

	bool m_bStereoLinkControlEditable = false;
	bool m_bHighImpendanceControlEditable = false;
	bool m_bPhantomPowerControlEditable = false;
	bool m_bMuteControlEditable = false;
	bool m_bVolumeControlEditable = false;

	double m_fMinVolumeValue = 0.0;
	double m_fMaxVolumeValue = 0.0;
	double m_fVolumeResolution = 0.0;
	double m_fVolumePadValue = 0.0;
	double m_fMinTrimValue = 0.0;
	double m_fMaxTrimValue = 0.0;
};

#endif // RETSETAUDIOCONTROLDETAIL_H
