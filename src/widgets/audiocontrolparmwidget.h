#ifndef AUDIOCONTROLPARMWIDGET_H
#define AUDIOCONTROLPARMWIDGET_H

#include "../device.h"
#include "../sysex/getaudiocontrolparm.h"
#include "../sysex/retsetaudiocontrolparm.h"

#include <QWidget>

namespace Ui {
class AudioControlParmWidget;
}

class AudioControlParmWidget : public QWidget {
	Q_OBJECT

  public:
	explicit AudioControlParmWidget(Device *device, int m_iPortId,
									QWidget *parent = nullptr);
	~AudioControlParmWidget();

  private: // methods
	void loadAudioControlParms();
	void addAudioControlParm(RetSetAudioControlParm *retSetAudioControlParm);
	void addAudioControllerSections();

  private:
	Ui::AudioControlParmWidget *ui;
	Device *m_pDevice = nullptr;
	int m_iPortId = 0;

	GetAudioControlParm *m_pGetAudioControlParm = nullptr;

	QMap<AudioControllerType, QVector<RetSetAudioControlParm *> *>
		*m_pAudioControlParms = nullptr;
};

#endif // AUDIOCONTROLPARMWIDGET_H
