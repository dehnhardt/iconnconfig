#include "portroutingwidget.h"
#include "../sysex/getmidiportroute.h"

#include <sstream>

PortRoutingWidget::PortRoutingWidget(Device *device, int portNumber,
																		 QWidget *parent)
		: QWidget(parent), device(device), portNumber(portNumber) {
	buttonLines = new std::vector<std::vector<PortButton *> *>;
	retrieveData();
	createSignalMapper();
	createWidgets();
	connect(lineButtonMapper, SIGNAL(mapped(QObject *)), this,
					SLOT(lineButtonClicked(QObject *)));
}

PortRoutingWidget::~PortRoutingWidget() {
	delete lineButtonMapper;
	delete portButtonMapper;
}

void PortRoutingWidget::createMidiPorts(
		int line, std::vector<RetSetMidiPortInfo *> *midiPortInfos) {
	std::vector<RetSetMidiPortInfo *>::iterator it;
	std::vector<PortButton *> *buttonLine = new std::vector<PortButton *>();
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		RetSetMidiPortInfo *midiPortInfo = *it;
		int jackNumber = midiPortInfo->getPortNumberOfType();
		PortButton *p =
				new PortButton(midiPortInfo->getPortId(), QString::number(jackNumber),
											 QString(midiPortInfo->getPortName().c_str()),
											 midiPortInfo->getPortType(), this);
		p->setEnabled(midiPortInfo->getOutputEnabled());
		p->setChecked(midiPortRoute->isPortRouted(p->getValue()));
		layout->addWidget(p, line, jackNumber);
		buttonLine->push_back(p);
	}
	buttonLines->push_back(buttonLine);
}

void PortRoutingWidget::createMidiPortSections(Device *device) {
	int line = 0;
	MIDI_PORT_INFOS *midiPortInfoSections = device->getMidiPortInfos();
	std::map<int, std::vector<RetSetMidiPortInfo *> *>::iterator it;
	for (it = midiPortInfoSections->begin(); it != midiPortInfoSections->end();
			 ++it, ++line) {
		int portType = it->first;
		portType >>= 8;
		MidiPortType pt = (MidiPortType)portType;
		PortButton *bAll = new PortButton(
				0, "all", QString(PortDisplayHelper::getMidiPortTypeName(pt).c_str()),
				pt, this);
		bAll->setCheckable(false);
		connect(bAll, SIGNAL(clicked(bool)), lineButtonMapper, SLOT(map()));
		lineButtonMapper->setMapping(bAll, new PortButtonMapper(bAll));
		layout->addWidget(bAll, line, 0);
		std::vector<RetSetMidiPortInfo *> *midiPortInfos = it->second;
		createMidiPorts(line, midiPortInfos);
	}
}

void PortRoutingWidget::createWidgets() {
  layout = new QGridLayout(this);
	createMidiPortSections(device);
}

void PortRoutingWidget::setupWidgets() {}

void PortRoutingWidget::setupLayout() { setLayout(layout); }

void PortRoutingWidget::retrieveData() {
	GetMidiPortRoute *getMidiPortRoute = new GetMidiPortRoute(device);
	getMidiPortRoute->setPortNumer(portNumber);
	midiPortRoute = (RetSetMidiPortRoute *)getMidiPortRoute->query();
}

void PortRoutingWidget::createSignalMapper() {
	lineButtonMapper = new QSignalMapper();
	portButtonMapper = new QSignalMapper();
}

int PortRoutingWidget::getButtonLineIndex(PortButton *b) {
	int row, column, rowspan, colspan;
	int index = layout->indexOf(b);
	layout->getItemPosition(index, &row, &column, &rowspan, &colspan);
	return row;
}

void PortRoutingWidget::lineButtonClicked(QObject *object) {
	PortButtonMapper *m = (PortButtonMapper *)object;
	PortButton *b = m->portButton;
	int row = getButtonLineIndex(b);
	setButtonsChecked(row, !isButtonChecked(row));
}

bool PortRoutingWidget::isButtonChecked(int row) {
	std::vector<PortButton *> *buttonLine = buttonLines->at(row);
	for (std::vector<PortButton *>::iterator it = buttonLine->begin();
			 it != buttonLine->end(); ++it) {
		if ((*it)->isChecked())
			return true;
	}
	return false;
}

void PortRoutingWidget::setButtonsChecked(int row, bool checked) {
	std::vector<PortButton *> *buttonLine = buttonLines->at(row);
	for (std::vector<PortButton *>::iterator it = buttonLine->begin();
			 it != buttonLine->end(); ++it) {
		(*it)->setChecked(checked);
	}
}
