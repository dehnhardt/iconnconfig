#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include "../device.h"
#include "multiinfowidget.h"

#include <QDockWidget>

class AudioWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	explicit AudioWidget(MioMain *parent = nullptr, Device *device = nullptr,
						 QString windowTitle = tr("Audio"));
	~AudioWidget();

  protected:
	QWidget *createWidget(MultiInfoListEntry *entry);
};

#endif // AUDIOROUTINGSSWIDGET_H
