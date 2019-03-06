#include "portroutingwidget.h"
#include "../sysex/getmidiinfo.h"
#include "../sysex/getmidiportroute.h"

#include <QLabel>
#include <sstream>

PortRoutingWidget::PortRoutingWidget(Device *device, int portNumber,
									 QWidget *parent)
	: QWidget(parent), device(device), portNumber(portNumber) {
	buttonLines = new std::vector<std::vector<PortButton *> *>;
	updateTimer = new QTimer(this);
	updateTimer->setSingleShot(true);
	retrieveData();
	createWidgets();
	loadData();

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateRouting()));
}

PortRoutingWidget::~PortRoutingWidget() {}

void PortRoutingWidget::createMidiPorts(
	int line, std::vector<RetSetMidiPortInfo *> *midiPortInfos) {
	std::vector<RetSetMidiPortInfo *>::iterator it;
	std::vector<PortButton *> *buttonLine = new std::vector<PortButton *>();
	for (it = midiPortInfos->begin(); it != midiPortInfos->end(); ++it) {
		RetSetMidiPortInfo *midiPortInfo = *it;
		int jackNumber = midiPortInfo->getPortNumberOfType();
		PortButton *p = new PortButton(
			midiPortInfo->getPortId(), QString::number(jackNumber),
			QString(midiPortInfo->getPortName().c_str()),
			midiPortInfo->getPortType(), this);
		p->setEnabled(midiPortInfo->getOutputEnabled());
		connect(p, &PortButton::clicked, this,
				[this, p]() { portButtonClicked(p); });
		layout->addWidget(p, line, jackNumber + 1);
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
		int jack = portType & 255;
		portType >>= 8;
		MidiPortType pt = static_cast<MidiPortType>(portType);
		PortButton *bAll = new PortButton(
			0, "all",
			QString(PortDisplayHelper::getMidiPortTypeName(pt).c_str()), pt,
			this);
		bAll->setChecked(true);
		bAll->setCheckable(false);
		connect(bAll, &PortButton::clicked, this,
				[this, bAll]() { lineButtonClicked(bAll); });
		QString portName(PortDisplayHelper::getMidiPortTypeName(pt)
							 .append(" ")
							 .append(jack > 0 ? std::to_string(jack) : "")
							 .c_str());
		layout->addWidget(new QLabel(portName), line, 0);
		layout->addWidget(bAll, line, 1);
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
	getMidiPortRoute->setDebug(true);
	getMidiPortRoute->setPortNumer(portNumber);
	midiPortRoute =
		static_cast<RetSetMidiPortRoute *>(getMidiPortRoute->query());
	midiPortRoute->setTotalNumberOfPorts(device->getMidiInfo()->getMidiPorts());
	midiPortRoute->setDebug(true);
	midiPortRoute->setCmdflags(0x40);
}

void PortRoutingWidget::loadData() {
	std::vector<std::vector<PortButton *> *>::iterator lineIt;
	for (lineIt = buttonLines->begin(); lineIt != buttonLines->end();
		 ++lineIt) {
		std::vector<PortButton *>::iterator buttonIt;
		std::vector<PortButton *> *v = (*lineIt);
		for (buttonIt = v->begin(); buttonIt != v->end(); ++buttonIt) {
			PortButton *p = (*buttonIt);
			p->setChecked(
				midiPortRoute->isPortRouted(static_cast<int>(p->getValue())));
		}
	}
}

int PortRoutingWidget::getButtonLineIndex(PortButton *b) {
	int row, column, rowspan, colspan;
	int index = layout->indexOf(b);
	layout->getItemPosition(index, &row, &column, &rowspan, &colspan);
	return row;
}

void PortRoutingWidget::lineButtonClicked(PortButton *b) {
	int row = getButtonLineIndex(b);
	bool buttonChecked = isButtonChecked(row);
	setButtonsChecked(row, !buttonChecked);
}

void PortRoutingWidget::portButtonClicked(PortButton *b) {
	int portNumber = static_cast<int>(b->getValue());
	midiPortRoute->setPortRouted(portNumber, b->isChecked());
	std::cout << "changed port " << portNumber << " to " << b->isChecked()
			  << std::endl;
	updateTimer->start(1000);
}

bool PortRoutingWidget::isButtonChecked(int row) {
	std::vector<PortButton *> *buttonLine =
		buttonLines->at(static_cast<unsigned long>(row));
	for (std::vector<PortButton *>::iterator it = buttonLine->begin();
		 it != buttonLine->end(); ++it) {
		if ((*it)->isChecked())
			return true;
	}
	return false;
}

void PortRoutingWidget::setButtonsChecked(int row, bool checked) {
	std::vector<PortButton *> *buttonLine =
		buttonLines->at(static_cast<unsigned long>(row));
	for (std::vector<PortButton *>::iterator it = buttonLine->begin();
		 it != buttonLine->end(); ++it) {
		(*it)->setChecked(checked);
		int portNumber = static_cast<int>((*it)->getValue());
		midiPortRoute->setPortRouted(portNumber, checked);
	}
	updateTimer->start(1000);
}

void PortRoutingWidget::updateRouting() {
	std::cout << "update midiportrouting" << std::endl;
	midiPortRoute->execute();
}
