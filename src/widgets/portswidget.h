#ifndef PORTSWIDGET_H
#define PORTSWIDGET_H

#include "../device.h"
#include "../sysex/retsetmidiportinfo.h"
#include "multiinfowidget.h"

#include <QDockWidget>

class PortsWidget : public MultiInfoWidget {
	Q_OBJECT

  public:
	explicit PortsWidget(MioMain *parent = nullptr, Device *device = nullptr,
						 QString windowTitle = tr("MIDI-Ports"));
	~PortsWidget();

  protected:
	QWidget *createWidget(MultiInfoListEntry *entry);

  private:
	void getMidiPortSections(Device *device);
	void getMidiPorts(std::vector<RetSetMidiPortInfo *> *midiPortInfos);
};

#endif // PORTSWIDGET_H
