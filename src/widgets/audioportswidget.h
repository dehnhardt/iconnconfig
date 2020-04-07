#ifndef AUDIOPORTSWIDGET_H
#define AUDIOPORTSWIDGET_H

#include "../device.h"
#include "../sysex/retsetaudioportparm.h"
#include "multiinfowidget.h"

#include <QDockWidget>

class AudioPortsWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	explicit AudioPortsWidget(MioMain *parent = nullptr,
							  Device *m_pDevice = nullptr,
							  QString windowTitle = tr("Audio-Ports"));
	~AudioPortsWidget();

  protected:
	QWidget *createWidget(MultiInfoListEntry *entry);

  private:
	void getAudioPortSections();
	void retrieveAudioPorts();
	void getAudioPorts(
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *audioPortParms);
	std::shared_ptr<AudioPortStructure> m_pAudioPortParms = nullptr;
};

#endif // AUDIOPORTSWIDGET_H
