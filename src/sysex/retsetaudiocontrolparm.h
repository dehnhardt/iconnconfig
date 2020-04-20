#ifndef RETSETAUDIOCONTROLPARM_H
#define RETSETAUDIOCONTROLPARM_H

#include "portsysexmessage.h"

class RetSetAudioControlParm : public PortSysExMessage {
  public:
	RetSetAudioControlParm(Device *m_pDevice);

	RetSetAudioControlParm(Command cmd, BYTE_VECTOR *message, Device *device)
	    : PortSysExMessage(cmd, message, device) {}

  public:
	// methods
	void parseAnswerData() override;

	// SysExMessage interface
	std::vector<unsigned char> *getMessageData() override;

	std::string getControllerName() const;
	void setControllerName(const std::string &sControllerName);

	pk::AudioControllerType getControllerType() const;
	void setControllerType(const pk::AudioControllerType &iControllerType);

	unsigned int getControllerNumber() const;
	void setControllerNumber(unsigned int iControllerNumber);

	unsigned int getControllerNameLength() const;
	void setControllerNameLength(unsigned int iControllerNameLength);

	unsigned int getCurrentSelectorInput() const;
	void setCurrentSelectorInput(unsigned int iCurrentSelectorInput);

	unsigned int getNumberOfSelectorInputs() const;
	void setNumberOfSelectorInputs(unsigned int iNumberOfSelectorInputs);

	unsigned int getNumberOfFeatuireChannels() const;
	void setNumberOfFeatuireChannels(unsigned int iNumberOfFeatuireChannels);

	unsigned int getCurrenClockSourceInput() const;
	void setCurrenClockSourceInput(unsigned int iCurrenClockSourceInput);

	unsigned int getNumberOfClockSourceInputs() const;
	void setNumberOfClockSourceInputs(unsigned int iNumberOfClockSourceInputs);

  private: // members
	unsigned int m_iControllerNumber = 0;
	pk::AudioControllerType m_iControllerType = pk::AudioControllerType::CT_NONE;
	unsigned int m_iControllerNameLength = 0;
	std::string m_sControllerName;

	// Selector
	unsigned int m_iCurrentSelectorInput = 0;
	unsigned int m_iNumberOfSelectorInputs = 0;

	// Feature
	unsigned int m_iNumberOfFeatureChannels = 0;

	// Clock Source
	unsigned int m_iCurrenClockSourceInput = 0;
	unsigned int m_iNumberOfClockSourceInputs = 0;
};

#endif // RETSETAUDIOCONTROLPARM_H
