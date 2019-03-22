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

	bool hasFeatures() const;
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

	int getMinVolumeValue() const;
	int getMaxVolumeValue() const;
	int getVolumeResolution() const;
	int getVolumePadValue() const;
	int getMinTrimValue() const;
	int getMaxTrimValue() const;

	unsigned char getControllerNumber() const;
	unsigned char getDetailNumber() const;

	ChannelDirection getChannelDirection() const;

  private:
	void parseExistFlags(unsigned char exist_flags);
	void parseEditFlags(unsigned char edit_flags);
	void parseVolumeValues(BYTE_VECTOR *v, unsigned long &offset);

  private:
	unsigned char m_iControllerNumber = 0;
	unsigned char m_iDetailNumber = 0;
	AudioControllerType m_controllerType = AudioControllerType::CT_NONE;
	ChannelDirection m_channelDirection = CD_NONE;
	unsigned int m_iChannnelNumber = 0;
	unsigned int m_iNameLenght = 0;
	std::string m_sSelectInputName;
	std::string m_sChannelName;
	std::string m_sClockSourceName;

	// feature
	bool m_bHasFeatures = false;
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

	int m_iMinVolumeValue = 0;
	int m_iMaxVolumeValue = 0;
	int m_iVolumeResolution = 0;
	int m_iVolumePadValue = 0;
	int m_iMinTrimValue = 0;
	int m_iMaxTrimValue = 0;
};

#endif // RETSETAUDIOCONTROLDETAIL_H
