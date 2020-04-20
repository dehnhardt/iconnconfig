#ifndef RETSETMIXEROUTPUTCONTROLVALUE_H
#define RETSETMIXEROUTPUTCONTROLVALUE_H

#include "portsysexmessage.h"

class RetSetMixerOutputControlValue : public PortSysExMessage {
  public:
	RetSetMixerOutputControlValue(Device *m_pDevice);

	RetSetMixerOutputControlValue(Command cmd, BYTE_VECTOR *message,
								  Device *device)
		: PortSysExMessage(cmd, message, device) {}

	virtual ~RetSetMixerOutputControlValue() override;

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

	int getSolo() const;
	void setSolo(int solo);

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
	pk::AudioControllerType m_controllerType = pk::AudioControllerType::CT_NONE;
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
	int m_iSolo = 0;
	bool m_bSoloPFL = false;
	bool m_bSteroLink = false;
	bool m_bInvert = false;

	pk::PanCurve m_iPanCurve = pk::PanCurve::NO_CURVE;

	int m_iVolume = 0;
	int m_iPan = 0;
};

#endif // RETSETMIXEROUTPUTCONTROLVALUE_H
