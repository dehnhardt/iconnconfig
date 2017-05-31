#include "portswidget.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetmidiinfo.h"
#include "portdisplayhelper.h"
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
		getMidiPortSections(device);
	}
}

PortsWidget::~PortsWidget() {}

void PortsWidget::getMidiPorts(
		std::vector<RetSetMidiPortInfo *> *midiPortInfos) {
	std::vector<RetSetMidiPortInfo *>::iterator it;
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		RetSetMidiPortInfo *midiPortInfo = *it;
		MultiInfoListEntry *entry = new MultiInfoListEntry(
				MultiInfoListEntry::PORT_ROUTING, midiPortInfo->getPortName());
		entry->icon = PortDisplayHelper::getPortIcon(midiPortInfo->getPortType());
		infoSections->push_back(entry);
	}
}

void PortsWidget::getMidiPortSections(Device *device) {
	MIDI_PORT_INFOS *midiPortInfoSections = device->getMidiPortInfos();
	std::map<int, std::vector<RetSetMidiPortInfo *> *>::iterator it;
	for (it = midiPortInfoSections->begin(); it != midiPortInfoSections->end();
			 ++it) {
		MidiPortType portType = (MidiPortType)it->first;
		std::string portTypeName = PortDisplayHelper::getMidiPortTypeName(portType);
		infoSections->push_back(
				new MultiInfoListEntry(MultiInfoListEntry::SECTION, portTypeName));
		std::vector<RetSetMidiPortInfo *> *midiPortInfos = it->second;
		getMidiPorts(midiPortInfos);
	}
}

QWidget *PortsWidget::createWidget(MultiInfoListEntry *entry) {
	std::cout << "create PortsWidget" << std::endl;
	PortRoutingWidget *w =
			new PortRoutingWidget(device, this->parentWidget(), countDinPorts);
  QScrollArea *a = new QScrollArea(this);
  a->setWidget(w);
  return a;
}
