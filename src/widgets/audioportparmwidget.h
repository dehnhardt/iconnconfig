#ifndef AUDIOPORTPARMWIDGET_H
#define AUDIOPORTPARMWIDGET_H

#include "../sysex/retsetaudioportparm.h"

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

  public slots:
	void audioConfigurationChanged();

  private:
	Ui::AudioPortParmWidget *ui;
	RetSetAudioPortParm *m_pRetSetAudioPortParm = nullptr;
};

#endif // AUDIOPORTPARMWIDGET_H
