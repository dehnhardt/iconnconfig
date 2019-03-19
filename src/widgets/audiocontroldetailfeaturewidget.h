#ifndef AUDIOCONTROLDETAILFEATUREWIDGET_H
#define AUDIOCONTROLDETAILFEATUREWIDGET_H

#include "../sysex/retsetaudiocontrolparm.h"

#include <QLayout>
#include <QObject>
#include <QWidget>

class AudioControlDetailFeatureWidget : public QWidget {
	Q_OBJECT
  public:
	explicit AudioControlDetailFeatureWidget(
		RetSetAudioControlParm *retSetAudioControlParm, Device *device,
		QWidget *parent = nullptr);

	void getDetails();

  private:
	void createLayout();

	RetSetAudioControlParm *m_pRetSetAudioControlParm = nullptr;
	Device *m_pDevice = nullptr;
	unsigned int m_iPortId = 0;
	unsigned int m_iControllerNumber = 0;
	QHBoxLayout *m_pLayout = nullptr;
  signals:

  public slots:
};

#endif // AUDIOCONTROLDETAILFEATUREWIDGET_H
