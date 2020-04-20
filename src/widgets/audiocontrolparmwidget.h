#ifndef AUDIOCONTROLPARMWIDGET_H
#define AUDIOCONTROLPARMWIDGET_H

#include "../device.h"
#include "../sysex/getaudiocontrolparm.h"
#include "../sysex/retsetaudiocontrolparm.h"
#include "../sysex/retsetaudiodeviceparm.h"

#include <QLayout>
#include <QTabWidget>
#include <QWidget>

class AudioControlParmWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioControlParmWidget(
		Device *device, unsigned int m_iPortId,
		std::shared_ptr<RetSetAudioDeviceParm> retSetAudioDeviceParm,
		QWidget *parent = nullptr);
	~AudioControlParmWidget();

  private: // methods
	void createLayout();
	void loadAudioControlParms();
	void addAudioControlParm(
		std::shared_ptr<RetSetAudioControlParm> retSetAudioControlParm);
	void addAudioControllerSections();

  private:
	Device *m_pDevice = nullptr;
	unsigned int m_iPortId = 0;
	std::shared_ptr<RetSetAudioDeviceParm> m_pRetSetAudioDeviceParm = nullptr;
	QLayout *m_pLayout = nullptr;
	QTabWidget *m_pFeatureTabWidget = nullptr;
	QString getFeatureName(pk::AudioControllerType audioControllerType);

	QMap<pk::AudioControllerType, QVector<std::shared_ptr<RetSetAudioControlParm>>
								  *> *m_pAudioControlParms = nullptr;
};

#endif // AUDIOCONTROLPARMWIDGET_H
