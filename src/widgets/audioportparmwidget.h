#ifndef AUDIOPORTPARMWIDGET_H
#define AUDIOPORTPARMWIDGET_H

#include "../sysex/retsetaudioportparm.h"

#include <QRegExpValidator>
#include <QTimer>
#include <QWidget>

namespace Ui {
class AudioPortParmWidget;
}

class AudioPortParmWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioPortParmWidget(
		std::shared_ptr<RetSetAudioPortParm> retstetaudioportparm,
		QWidget *parent = nullptr);
	~AudioPortParmWidget();
	void setData();
	void setCurrentAudioConfiguration();

  private:
	Ui::AudioPortParmWidget *ui;
	QTimer *m_pUpdateTimer = nullptr;
	QRegExpValidator *m_pRegExpValidator = nullptr;

	std::shared_ptr<RetSetAudioPortParm> m_pRetSetAudioPortParm;
	void createConnections();
	bool checkTotalNumberOfAudioChannels();

  public slots:
	void audioConfigurationChanged();

  signals:
	void changePortName(std::string portName);
};

#endif // AUDIOPORTPARMWIDGET_H
