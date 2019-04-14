#ifndef AUDIOCONTROLPARMFEATURESWIDGET_H
#define AUDIOCONTROLPARMFEATURESWIDGET_H

#include "../sysex/getaudioportmetervalue.h"
#include "../sysex/retsetaudiocontrolparm.h"

#include <QLayout>
#include <QObject>
#include <QTabWidget>
#include <QVector>
#include <QWidget>

class AudioControlParmFeaturesWidget : public QWidget {
	Q_OBJECT
  public:
	explicit AudioControlParmFeaturesWidget(
		Device *device,
		QVector<std::shared_ptr<RetSetAudioControlParm>> *features,
		QWidget *parent = nullptr);
	~AudioControlParmFeaturesWidget() override;

  private:
	QTabWidget *m_pControllerNameTabs = nullptr;
	Device *m_pDevice = nullptr;
	QGridLayout *m_pLayout = nullptr;
	QTimer *m_pVolumeTimer = nullptr;

	void createLayout();
	void addFeatures();
	void createFeatureWidget(
		std::shared_ptr<RetSetAudioControlParm> retSetAudioControlParm);
	std::shared_ptr<GetAudioPortMeterValue> m_pGetAudioPortMeterValue;
	QVector<std::shared_ptr<RetSetAudioControlParm>> *m_pFeatures = nullptr;

  signals:
	void inMeterValueChanged(int channel, int value);
	void outMeterValueChanged(int channel, int value);

  private slots:
	void timerElapsed();

  public slots:

	// QWidget interface
  protected:
	void showEvent(QShowEvent *event) override;
	void hideEvent(QHideEvent *event) override;
};

#endif // AUDIOCONTROLPARMFEATURESWIDGET_H
