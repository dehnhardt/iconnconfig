#ifndef RETSETAUDIOGLOBALPARM_H
#define RETSETAUDIOGLOBALPARM_H

#include "sysexmessage.h"

#include <QApplication>

typedef struct {
	int configurationNumber;
	int bitDepthCode;
	int sampleRateCode;
} AUDIOConfiguration;

class RetSetAudioGlobalParm : public SysExMessage {
	Q_DECLARE_TR_FUNCTIONS(RetSetAudioGlobalParm)

  public:
	RetSetAudioGlobalParm(Device *device);
	RetSetAudioGlobalParm(Command cmd, BYTE_VECTOR *message, Device *device)
		: SysExMessage(cmd, message, device) {}
	~RetSetAudioGlobalParm() override;
	int getSettingsId() override { return m_Command; }
	int getSettingsIndex() override { return 0; }
	std::string getStorableValue() override { return ""; }

	std::string getAudioConfigurationString(unsigned int i);
	std::string getAudioConfigurationString();

	BYTE_VECTOR *getMessageData() override;

	unsigned int translateBitDepth(int bitCode);
	std::string translateBitDepthText(int bitCode);

	unsigned int translateSampleRate(int sampleRateCode);
	std::string translateSampleRateText(int sampleRateCode);

	// Getter & Setter
	unsigned int getNumberOfAudioPorts() const;

	unsigned int getMinAudioFramesBuffered() const;

	unsigned int getMaxAudioFramesBuffered() const;

	unsigned int getCurrentAudioFramesBuffered() const;
	void
	setCurrentAudioFramesBuffered(unsigned int iCurrentAudioFramesBuffered);

	unsigned int getMinAllowedSyncFactor() const;

	unsigned int getMaxAllowedSyncFactor() const;

	unsigned int getCurrentSyncFactor() const;
	void setCurrentSyncFactor(unsigned int iCurrentSyncFactor);

	unsigned int getNumberOfActiveAudioConfiguration() const;
	void setNumberOfActiveAudioConfiguration(
		unsigned int iNumberOfActiveAudioConfigurations);

	unsigned int getNumberOfConfigBlocks() const;

	AUDIOConfiguration *getAudioConfiguration(int i) const;
	void setAudioConfiguration(AUDIOConfiguration *pAudioConfigurations, int i);

  private:
	void parseAnswerData() override;

	unsigned int m_iNumberOfAudioPorts = 0;
	unsigned int m_iMinAudioFramesBuffered = 0;
	unsigned int m_iMaxAudioFramesBuffered = 0;
	unsigned int m_iCurrentAudioFramesBuffered = 0;
	unsigned int m_iMinAllowedSyncFactor = 0;
	unsigned int m_iMaxAllowedSyncFactor = 0;
	unsigned int m_iCurrentSyncFactor = 0;
	unsigned int m_iNumberOfActiveAudioConfiguration = 0;
	unsigned int m_iNumberOfConfigBlocks = 0;
	AUDIOConfiguration **m_pAudioConfigurations = nullptr;
};

#endif // RETSETAUDIOGLOBALPARM_H
