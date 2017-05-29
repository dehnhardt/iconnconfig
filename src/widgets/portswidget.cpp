#include "portswidget.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetmidiinfo.h"
#include "portroutingwidget.h"

#include <QLabel>
#include <QScrollArea>

PortsWidget::PortsWidget(MioMain *parent, Device *device, QString windowTitle)
    : MultiInfoWidget(parent, device, windowTitle) {
  infoSections = new std::vector<MultiInfoListEntry *>();
  if (device->getCommands()->isCommandSupported(
          SysExMessage::GET_MIDI_PORT_ROUTE)) {
    RetSetMidiInfo *midiInfo = device->getMidiInfo();
    countDinPorts = midiInfo->getDinPorts();
    for (int i = 0; i < countDinPorts; ++i) {
      infoSections->push_back(new MultiInfoListEntry(
          MultiInfoListEntry::PORT_ROUTING, tr("MIDI-Port").toStdString(), i));
    }
  }
}

PortsWidget::~PortsWidget() {}

QWidget *PortsWidget::createWidget(MultiInfoListEntry *entry) {
  PortRoutingWidget *w =
      new PortRoutingWidget(this->parentWidget(), countDinPorts);
  QScrollArea *a = new QScrollArea(this);
  a->setWidget(w);
  return a;
}
