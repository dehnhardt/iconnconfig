#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

#include <QDockWidget>

class AudioWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	AudioWidget(MioMain *parent = nullptr, Device *m_pDevice = nullptr,
				QString windowTitle = tr("Audio Routing"));
	~AudioWidget();

  protected:
	QWidget *createWidget(MultiInfoListEntry *entry);
};

#endif // AUDIOROUTINGSSWIDGET_H
