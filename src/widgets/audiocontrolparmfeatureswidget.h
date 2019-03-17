#ifndef AUDIOCONTROLPARMFEATURESWIDGET_H
#define AUDIOCONTROLPARMFEATURESWIDGET_H

#include "../sysex/retsetaudiocontrolparm.h"

#include <QLayout>
#include <QObject>
#include <QTabWidget>
#include <QVector>
#include <QWidget>

class AudioControlParmFeaturesWidget : public QTabWidget {
	Q_OBJECT
  public:
	explicit AudioControlParmFeaturesWidget(
		Device *device, QVector<RetSetAudioControlParm *> *features,
		QWidget *parent = nullptr);

  private:
	QTabWidget *m_pControllerNameTabs = nullptr;
	Device *m_pDevice = nullptr;

	void createLayout();
	void addFeatures();
	void createFeatureWidget(RetSetAudioControlParm *retSetAudioControlParm);
	QVector<RetSetAudioControlParm *> *m_pFeatures = nullptr;

  signals:

  public slots:
};

#endif // AUDIOCONTROLPARMFEATURESWIDGET_H
