#ifndef AUDIOPORTSWIDGET_H
#define AUDIOPORTSWIDGET_H

#include "../device.h"
#include "../sysex/retsetaudioportparm.h"
#include "multiinfowidget.h"

#include <QDockWidget>

typedef std::map<int, std::vector<RetSetAudioPortParm *> *> AudioPortStructure;

class AudioPortsWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	explicit AudioPortsWidget(MioMain *parent = nullptr,
							  Device *device = nullptr,
							  QString windowTitle = tr("Audio-Ports"));
	~AudioPortsWidget();

  protected:
	QWidget *createWidget(MultiInfoListEntry *entry);

  private:
	void getAudioPortSections();
	void retrieveAudioPorts();
	void getAudioPorts(std::vector<RetSetAudioPortParm *> *audioPortParms);
	AudioPortStructure *m_pAudioPortParms = nullptr;
};

#endif // AUDIOPORTSWIDGET_H
