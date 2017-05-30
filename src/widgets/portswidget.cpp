#include "portswidget.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetmidiinfo.h"
#include "portroutingwidget.h"

#include <QLabel>
#include <QScrollArea>

std::string PortsWidget::getPortTypeName(MidiPortType portType) {
	std::string portTypeName;
	switch (portType) {
	case NONE:
		break;
	case DIN:
		portTypeName = "DIN-Port";
		break;
	case USB_DEVICE:
		portTypeName = "USB-Device-Port";
		break;
	case USB_HOST:
		portTypeName = "USB-Host-Port";
		break;
	case ETHERNET:
		portTypeName = "Ethernet-Port";
		break;
	}
	return portTypeName;
}

void PortsWidget::getMidiPorts(
		std::vector<RetSetMidiPortInfo *> *midiPortInfos) {
	std::vector<RetSetMidiPortInfo *>::iterator it;
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		RetSetMidiPortInfo *midiPortInfo = *it;
		infoSections->push_back(new MultiInfoListEntry(
				MultiInfoListEntry::PORT_ROUTING, midiPortInfo->getPortName()));
	}
}

void PortsWidget::getMidiPortSections(Device *device) {
	MIDI_PORT_INFOS *midiPortInfoSections = device->getMidiPortInfos();
	std::map<int, std::vector<RetSetMidiPortInfo *> *>::iterator it;
	for (it = midiPortInfoSections->begin(); it != midiPortInfoSections->end();
			 ++it) {
		MidiPortType portType = (MidiPortType)it->first;
		std::string portTypeName = getPortTypeName(portType);
		infoSections->push_back(
				new MultiInfoListEntry(MultiInfoListEntry::SECTION, portTypeName));
		std::vector<RetSetMidiPortInfo *> *midiPortInfos = it->second;
		getMidiPorts(midiPortInfos);
	}
}

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

QWidget *PortsWidget::createWidget(MultiInfoListEntry *entry) {
  PortRoutingWidget *w =
      new PortRoutingWidget(this->parentWidget(), countDinPorts);
  QScrollArea *a = new QScrollArea(this);
  a->setWidget(w);
  return a;
}
