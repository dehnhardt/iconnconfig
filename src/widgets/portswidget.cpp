#include "portswidget.h"
#include "../sysex/retcommandlist.h"
#include "../sysex/retsetmidiinfo.h"
#include "portdisplayhelper.h"
#include "portfilterwidget.h"
#include "portinfowidget.h"
#include "portremapwidget.h"
#include "portroutingwidget.h"

#include <QLabel>
#include <QListWidgetItem>
#include <QScrollArea>

PortsWidget::PortsWidget(MioMain *parent, Device *device, QString windowTitle)
	: MultiInfoWidget(parent, device, windowTitle) {
	m_pInfoSections = new std::vector<MultiInfoListEntry *>();
	if (device->getCommands()->isCommandSupported(
			Command::GET_MIDI_PORT_INFO)) {
		getMidiPortSections(device);
	}
}

PortsWidget::~PortsWidget() {
	/* if (infoSections != nullptr) {
		infoSections->clear();
		delete infoSections;
	}*/
}

void PortsWidget::getMidiPorts(
	std::vector<std::shared_ptr<RetSetMidiPortInfo>> *midiPortInfos) {
	std::vector<std::shared_ptr<RetSetMidiPortInfo>>::iterator it;
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		std::shared_ptr<RetSetMidiPortInfo> midiPortInfo = *it;
		MultiInfoListEntry *entry = new MultiInfoListEntry(
			MultiInfoListEntry::PORT_ROUTING, midiPortInfo->getPortName());
		entry->icon =
			PortDisplayHelper::getPortIcon(midiPortInfo->getPortType());
		entry->message = midiPortInfo;
		// TODO: mark port as disabled
		// entry->enabled = midiPortInfo->getInputEnabled();
		m_pInfoSections->push_back(entry);
	}
}

void PortsWidget::getMidiPortSections(Device *device) {
	MidiPortInfos *midiPortInfoSections = device->getMidiPortInfos();
	std::map<int, std::vector<std::shared_ptr<RetSetMidiPortInfo>> *>::iterator
		it;
	for (it = midiPortInfoSections->begin(); it != midiPortInfoSections->end();
		 ++it) {
		int section = it->first;
		int jack = section & 255;
		MidiPortType portType = static_cast<MidiPortType>(section >> 8);
		std::string portTypeName =
			PortDisplayHelper::getMidiPortTypeName(portType);
		if (jack > 0)
			portTypeName += " " + std::to_string(jack);
		m_pInfoSections->push_back(
			new MultiInfoListEntry(MultiInfoListEntry::SECTION, portTypeName));
		std::vector<std::shared_ptr<RetSetMidiPortInfo>> *midiPortInfos =
			it->second;
		getMidiPorts(midiPortInfos);
	}
}

QWidget *PortsWidget::createWidget(MultiInfoListEntry *entry) {
	std::cout << "create PortsWidget" << std::endl;
	std::shared_ptr<RetSetMidiPortInfo> midiPortInfo =
		std::dynamic_pointer_cast<RetSetMidiPortInfo>(entry->message);
	int portNumber = static_cast<int>(midiPortInfo->getPortId());
	QTabWidget *portTabWidget = new QTabWidget(this);
	PortInfoWidget *portInfoWidget = new PortInfoWidget(midiPortInfo);
	connect(portInfoWidget, &PortInfoWidget::changePortName, entry,
			&MultiInfoListEntry::changeName);
	portTabWidget->addTab(portInfoWidget, "MIDI-Port Info");
	if (m_pDevice->getCommands()->isCommandSupported(
			Command::GET_MIDI_PORT_ROUTE)) {
		PortRoutingWidget *w =
			new PortRoutingWidget(m_pDevice, portNumber, this->parentWidget());
		QScrollArea *a = new QScrollArea(portTabWidget);
		a->setWidget(w);
		portTabWidget->addTab(a, tr("MIDI-Port Routing"));
	}
	// TODO: smart_pointer
	if (m_pDevice->getCommands()->isCommandSupported(
			Command::GET_MIDI_PORT_FILTER)) {
		PortFilterWidget *portFilterWidget =
			new PortFilterWidget(m_pDevice, portNumber, this->parentWidget());
		portTabWidget->addTab(portFilterWidget, tr("MIDI-Port Filter"));
	}
	if (m_pDevice->getCommands()->isCommandSupported(
			Command::GET_MIDI_PORT_REMAP)) {
		PortRemapWidget *portRemapWidget =
			new PortRemapWidget(m_pDevice, portNumber, this->parentWidget());
		portTabWidget->addTab(portRemapWidget, tr("MIDI-Port Remap"));
	}
	return portTabWidget;
}
