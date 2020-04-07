#ifndef RETSETMIXERINPUTCONTROLVALUE_H
#define RETSETMIXERINPUTCONTROLVALUE_H

#include "portsysexmessage.h"

class RetSetMixerInputControlValue : public PortSysExMessage {
  public:
	RetSetMixerInputControlValue(Device *m_pDevice);

	RetSetMixerInputControlValue(Command cmd, BYTE_VECTOR *message,
								 Device *device)
		: PortSysExMessage(cmd, message, device) {}

	virtual ~RetSetMixerInputControlValue() override;

	void parseAnswerData() override;

	// SysExMessage interface
	std::vector<unsigned char> *getMessageData() override;

	void clean();

	bool hasStereoLinkControl() const;
	void setHasStereoLinkControl(bool hasStereoLinkControl);

	bool hasSoloControl() const;
	void setHasSoloControl(bool hasSoloControl);

	bool hasSoloPFLControl() const;
	void setHasSoloPFLControl(bool bHasSoloPFLControl);

	bool hasMuteControl() const;
	void setHasMuteControl(bool hasMuteControl);

	bool hasVolumeControl() const;
	void setHasVolumeControl(bool hasVolumeControl);

	bool hasPanControl() const;
	void setHasPanControl(bool bHasPanControl);

	bool hasInvertControl() const;
	void setHasInvertControl(bool bHasInvertControl);

	// value getter / setter
	bool getSteroLink() const;
	void setSteroLink(bool getSteroLink);

	bool getSolo() const;
	void setSolo(bool solo);

	bool getSoloPFL() const;
	void setSoloPFL(bool bSoloPFL);

	bool getInvert() const;
	void setInvert(bool bInvert);

	bool getMute() const;
	void setMute(bool getMute);

	int getVolume() const;
	void setVolume(int getVolume);

	int getPan() const;
	void setPan(int pan);

	unsigned char getMixerOutputNumber() const;

	unsigned char getMixerInputNumber() const;

  private:
	void parseExistFlags(unsigned char exist_flags);

  private:
	unsigned char m_iMixerOutputNumber = 0;
	unsigned char m_iMixerInputNumber = 0;
	AudioControllerType m_controllerType = CT_NONE;
	unsigned int m_iNameLenght = 0;
	std::string m_sClockSourceName;

	bool m_bHasPanControl = false;
	bool m_bHasInvertControl = false;
	bool m_bHasStereoLinkControl = false;
	bool m_bHasSoloControl = false;
	bool m_bHasSoloPFLControl = false;
	bool m_bHasMuteControl = false;
	bool m_bHasVolumeControl = false;

	bool m_bMute = false;
	bool m_bSolo = false;
	bool m_bSoloPFL = false;
	bool m_bSteroLink = false;
	bool m_bInvert = false;

	PanCurve m_iPanCurve = PanCurve::NO_CURVE;

	int m_iVolume = 0;
	int m_iPan = 0;
};

#endif // RETSETMIXERINPUTCONTROLVALUE_H
