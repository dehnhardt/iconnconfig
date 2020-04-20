#ifndef RETSETAUDIOCONTROLDETAILVALUE_H
#define RETSETAUDIOCONTROLDETAILVALUE_H

#include "portsysexmessage.h"

class RetSetAudioControlDetailValue : public PortSysExMessage {
  public:
	RetSetAudioControlDetailValue(Device *m_pDevice);

	RetSetAudioControlDetailValue(Command cmd, BYTE_VECTOR *message,
								  Device *device)
		: PortSysExMessage(cmd, message, device) {}
	virtual ~RetSetAudioControlDetailValue() override;

	void parseAnswerData() override;

	// SysExMessage interface
	std::vector<unsigned char> *getMessageData() override;

	void reset();

	bool hasStereoLinkControl() const;
	void setHasStereoLinkControl(bool hasStereoLinkControl);

	bool hasHighImpendanceControl() const;
	void setHasHighImpendanceControl(bool hasHighImpendanceControl);

	bool hasPhantomPowerControl() const;
	void setHasPhantomPowerControl(bool hasPhantomPowerControl);

	bool hasMuteControl() const;
	void setHasMuteControl(bool hasMuteControl);

	bool hasVolumeControl() const;
	void setHasVolumeControl(bool hasVolumeControl);

	bool getSteroLink() const;
	void setSteroLink(bool getSteroLink);

	bool getHigImpedance() const;
	void setHigImpedance(bool getHigImpedance);

	bool getPhantomPower() const;
	void setPhantomPower(bool getPhantomPower);

	bool getMute() const;
	void setMute(bool getMute);

	int getVolume() const;
	void setVolume(int getVolume);

	int getTrim() const;
	void setTrim(int getTrim);

  private:
	void parseExistFlags(unsigned char exist_flags);

  private:
	unsigned char m_iControllerNumber = 0;
	unsigned char m_iDetailNumber = 0;
	pk::AudioControllerType m_controllerType = pk::AudioControllerType::CT_NONE;
	unsigned int m_iNameLenght = 0;
	std::string m_sClockSourceName;

	bool m_bHasStereoLinkControl = false;
	bool m_bHasHighImpendanceControl = false;
	bool m_bHasPhantomPowerControl = false;
	bool m_bHasMuteControl = false;
	bool m_bHasVolumeControl = false;

	bool m_bSteroLink = false;
	bool m_bHigImpedance = false;
	bool m_bPhantomPower = false;
	bool m_bMute = false;

	int m_iVolume = 0;
	int m_iTrim = 0;
};

#endif // RETSETAUDIOCONTROLDETAILVALUE_H
