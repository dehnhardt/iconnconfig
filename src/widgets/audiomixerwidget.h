#ifndef AUDIOMIXERWIDGET_H
#define AUDIOMIXERWIDGET_H

#include "../device.h"
#include "../sysex/retsetmixerparm.h"
#include "../sysex/retsetmixerportparm.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>

class AudioMixerWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioMixerWidget(Device *d, QWidget *parent = nullptr);
	~AudioMixerWidget();

  private:
	Device *m_pDevice = nullptr;
	unsigned int m_iNumberOfAudioPorts = 0;
	std::shared_ptr<RetSetMixerParm> m_pRetSetMixerParm = nullptr;
	std::shared_ptr<RetSetMixerPortParm> m_pRetSetMixerPortParm = nullptr;

	QComboBox *m_pCboMixerConfig = nullptr;
	QScrollArea *m_pSAMixer = nullptr;
	QWidget *m_pWdgMixer = nullptr;
	QTabWidget *m_pMixerPartsTab = nullptr;
	QHBoxLayout *m_pLAInputMixerParts = nullptr;
	QHBoxLayout *m_pLAOutputMixerParts = nullptr;

	void createLayout();
	void createConnection();
	void createMixer();

  public slots:
	void audioConfigurationChanged(unsigned int audioConfigurationNumber);
	void mixerConfigurationChanged(unsigned int mixerConfigurationNumber);
	void changeChannelConnection();

  signals:
	void channelConnectionChanged();
};

#endif // AUDIOMIXERWIDGET_H
