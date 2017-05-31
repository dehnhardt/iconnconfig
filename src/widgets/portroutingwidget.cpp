#include "portroutingwidget.h"

#include <sstream>

PortRoutingWidget::PortRoutingWidget(Device *device, QWidget *parent,
																		 int numDinPorts)
		: QWidget(parent), device(device), numDinPorts(numDinPorts) {
  createWidgets();
}

void PortRoutingWidget::getMidiPorts(
		int line, std::vector<RetSetMidiPortInfo *> *midiPortInfos) {
	std::vector<RetSetMidiPortInfo *>::iterator it;
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		RetSetMidiPortInfo *midiPortInfo = *it;
		int jackNumber = midiPortInfo->getPortNumberOfType();
		PortButton *p = new PortButton(midiPortInfo->getPortId(), jackNumber,
																	 QString(midiPortInfo->getPortName().c_str()),
																	 midiPortInfo->getPortType(), this);
		layout->addWidget(p, line, jackNumber);
	}
}

void PortRoutingWidget::getMidiPortSections(Device *device) {
	int line = 0;
	MIDI_PORT_INFOS *midiPortInfoSections = device->getMidiPortInfos();
	std::map<int, std::vector<RetSetMidiPortInfo *> *>::iterator it;
	for (it = midiPortInfoSections->begin(); it != midiPortInfoSections->end();
			 ++it, ++line) {
		std::vector<RetSetMidiPortInfo *> *midiPortInfos = it->second;
		getMidiPorts(line, midiPortInfos);
	}
}

void PortRoutingWidget::createWidgets() {
  layout = new QGridLayout(this);
	getMidiPortSections(device);
}

void PortRoutingWidget::setupWidgets() {}

void PortRoutingWidget::setupLayout() { setLayout(layout); }

void PortRoutingWidget::setData() {}

void PortRoutingWidget::createConnections() {}
