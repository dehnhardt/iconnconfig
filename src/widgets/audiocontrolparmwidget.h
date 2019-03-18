#ifndef AUDIOCONTROLPARMWIDGET_H
#define AUDIOCONTROLPARMWIDGET_H

#include "../device.h"
#include "../sysex/getaudiocontrolparm.h"
#include "../sysex/retsetaudiocontrolparm.h"

#include <QLayout>
#include <QTabWidget>
#include <QWidget>

class AudioControlParmWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioControlParmWidget(Device *device, unsigned int m_iPortId,
									QWidget *parent = nullptr);
	~AudioControlParmWidget();

  private: // methods
	void createLayout();
	void loadAudioControlParms();
	void addAudioControlParm(RetSetAudioControlParm *retSetAudioControlParm);
	void addAudioControllerSections();

  private:
	Device *m_pDevice = nullptr;
	unsigned int m_iPortId = 0;
	QLayout *m_pLayout = nullptr;
	QTabWidget *m_pFeatureTabWidget = nullptr;
	QString getFeatureName(AudioControllerType audioControllerType);

	GetAudioControlParm *m_pGetAudioControlParm = nullptr;

	QMap<AudioControllerType, QVector<RetSetAudioControlParm *> *>
		*m_pAudioControlParms = nullptr;
};

#endif // AUDIOCONTROLPARMWIDGET_H
