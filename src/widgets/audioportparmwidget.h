#ifndef AUDIOPORTPARMWIDGET_H
#define AUDIOPORTPARMWIDGET_H

#include "../sysex/retsetaudioportparm.h"

#include <QTimer>
#include <QWidget>

namespace Ui {
class AudioPortParmWidget;
}

class AudioPortParmWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioPortParmWidget(RetSetAudioPortParm *retstetaudioportparm,
								 QWidget *parent = nullptr);
	~AudioPortParmWidget();
	void setData();
	void setCurrentAudioConfiguration();

  private:
	Ui::AudioPortParmWidget *ui;
	RetSetAudioPortParm *m_pRetSetAudioPortParm = nullptr;
	QTimer *m_pUpdateTimer = nullptr;

	void createConnections();
	bool checkTotalNumberOfAudioChannels();

  public slots:
	void audioConfigurationChanged();
};

#endif // AUDIOPORTPARMWIDGET_H
