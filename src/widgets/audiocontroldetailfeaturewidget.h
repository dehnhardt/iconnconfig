#ifndef AUDIOCONTROLDETAILFEATUREWIDGET_H
#define AUDIOCONTROLDETAILFEATUREWIDGET_H

#include "../sysex/retsetaudiocontrolparm.h"
#include "audiochannelfeaturewidget.h"

#include <QLayout>
#include <QObject>
#include <QWidget>

class AudioControlParmFeaturesWidget;

class AudioControlDetailFeatureWidget : public QWidget {
	Q_OBJECT
  public:
	explicit AudioControlDetailFeatureWidget(
		RetSetAudioControlParm *retSetAudioControlParm, Device *device,
		QWidget *parent);

	void getDetails();
	static int OUTPUT;
	static int INPUT;

  private:
	void createLayout();

	RetSetAudioControlParm *m_pRetSetAudioControlParm = nullptr;
	AudioControlParmFeaturesWidget *m_pAudioControlParmFeaturesWidget = nullptr;
	Device *m_pDevice = nullptr;
	unsigned int m_iPortId = 0;
	unsigned int m_iControllerNumber = 0;
	QHBoxLayout *m_pLayout = nullptr;
	std::vector<AudioChannelFeatureWidget *> m_vFeatureWidgets;

  signals:
	void meterValueChanged(int channel, int value);
  private slots:

  public slots:
	void linkStatusChanged(unsigned int detailId, bool status);
	void changeMeterValue(int channel, int value);
};

#endif // AUDIOCONTROLDETAILFEATUREWIDGET_H
