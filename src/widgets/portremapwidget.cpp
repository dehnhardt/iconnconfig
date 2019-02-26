#include "portremapwidget.h"

#include <QLabel>

#include <src/sysex/getmidiportremap.h>

PortRemapWidget::PortRemapWidget(Device *device, int portNumber,
								 QWidget *parent)
	: QWidget(parent), device(device), portNumber(portNumber) {
	m_pUpdateTimerInRemap = new QTimer(this);
	m_pUpdateTimerOutRemap = new QTimer(this);
	m_pUpdateTimerInRemap->setSingleShot(true);
	m_pUpdateTimerOutRemap->setSingleShot(true);

	createWidgets();
	retrieveData();
	loadData();
	createConnections();
}

PortRemapWidget::~PortRemapWidget() {
	delete m_pUpdateTimerInRemap;
	delete m_pUpdateTimerOutRemap;
}

void PortRemapWidget::midiPortRemapUpdated(PortDirection direction) {
	switch (direction) {
	case PortDirection::INPUT:
		m_pUpdateTimerInRemap->start(1000);
		break;
	case PortDirection::OUTPUT:
		m_pUpdateTimerOutRemap->start(1000);
		break;
	}
}

void PortRemapWidget::updateInRemap() {
	std::cout << "updateInRemap" << std::endl;
	m_pMidiPortRemapIn->execute();
}

void PortRemapWidget::updateOutRemap() {
	std::cout << "updateOutRemap" << std::endl;
	m_pMidiPortRemapOut->execute();
}

void PortRemapWidget::loadData() {
	// Input Remap
	m_pInputRemapWidget->setMidiControllerRemap(
		m_pMidiPortRemapIn->getMidiPortRemap()->midiControllerRemap);
	m_pInputRemapWidget->setMidiChannelMessagesRemap(
		m_pMidiPortRemapIn->getMidiPortRemap()->midiChannelMessagesRemap);

	// Output Remap
	m_pOutputRemapWidget->setMidiControllerRemap(
		m_pMidiPortRemapOut->getMidiPortRemap()->midiControllerRemap);
	m_pOutputRemapWidget->setMidiChannelMessagesRemap(
		m_pMidiPortRemapOut->getMidiPortRemap()->midiChannelMessagesRemap);
}

void PortRemapWidget::createConnections() {
	connect(m_pInputRemapWidget, &PortRemapSettingsWidget::remapDataChanged,
			this, &PortRemapWidget::midiPortRemapUpdated);
	connect(m_pOutputRemapWidget, &PortRemapSettingsWidget::remapDataChanged,
			this, &PortRemapWidget::midiPortRemapUpdated);
	connect(m_pUpdateTimerInRemap, &QTimer::timeout, this,
			&PortRemapWidget::updateInRemap);
	connect(m_pUpdateTimerOutRemap, &QTimer::timeout, this,
			&PortRemapWidget::updateOutRemap);
}

void PortRemapWidget::createWidgets() {
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);
	m_pRemapDirectionTabWidget = new QTabWidget(this);
	m_pRemapDirectionTabWidget->setTabPosition(QTabWidget::West);
	m_pInputRemapWidget =
		new PortRemapSettingsWidget(PortDirection::INPUT, this);
	m_pOutputRemapWidget =
		new PortRemapSettingsWidget(PortDirection::OUTPUT, this);
	m_pRemapDirectionTabWidget->addTab(m_pInputRemapWidget, tr("Input Remap"));
	m_pRemapDirectionTabWidget->addTab(m_pOutputRemapWidget,
									   tr("Output Remap"));
	gridLayout->addWidget(m_pRemapDirectionTabWidget);
}

void PortRemapWidget::retrieveData() {
	GetMidiPortRemap *getMidiPortRemap = new GetMidiPortRemap(device);
	getMidiPortRemap->setDebug(true);
	getMidiPortRemap->setPortNumer(static_cast<unsigned int>(portNumber));
	getMidiPortRemap->setPortRemapDirection(PortDirection::INPUT);
	getMidiPortRemap->setDebug(true);
	getMidiPortRemap->setCmdflags(0x40);
	m_pMidiPortRemapIn =
		static_cast<RetSetMidiPortRemap *>(getMidiPortRemap->query());
	getMidiPortRemap->setPortRemapDirection(PortDirection::OUTPUT);
	m_pMidiPortRemapOut =
		static_cast<RetSetMidiPortRemap *>(getMidiPortRemap->query());
}
