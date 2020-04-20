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
	m_pUpdateTimerInRemap->deleteLater();
	m_pUpdateTimerOutRemap->deleteLater();
}

void PortRemapWidget::midiPortRemapUpdated(pk::PortDirection direction) {
	switch (direction) {
	case pk::PortDirection::INPUT:
		m_pUpdateTimerInRemap->start(1000);
		break;
	case pk::PortDirection::OUTPUT:
		m_pUpdateTimerOutRemap->start(1000);
		break;
	default:
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
		new PortRemapSettingsWidget(pk::PortDirection::INPUT, this);
	m_pOutputRemapWidget =
		new PortRemapSettingsWidget(pk::PortDirection::OUTPUT, this);
	m_pRemapDirectionTabWidget->addTab(m_pInputRemapWidget, tr("Input Remap"));
	m_pRemapDirectionTabWidget->addTab(m_pOutputRemapWidget,
									   tr("Output Remap"));
	gridLayout->addWidget(m_pRemapDirectionTabWidget);
}

void PortRemapWidget::retrieveData() {
	std::unique_ptr<GetMidiPortRemap> getMidiPortRemap =
		std::make_unique<GetMidiPortRemap>(device);
	getMidiPortRemap->setDebug(false);
	getMidiPortRemap->setPortNumer(static_cast<unsigned int>(portNumber));
	getMidiPortRemap->setPortRemapDirection(pk::PortDirection::INPUT);
	getMidiPortRemap->setDebug(false);
	getMidiPortRemap->setCmdflags(0x40);
	m_pMidiPortRemapIn = std::dynamic_pointer_cast<RetSetMidiPortRemap>(
		getMidiPortRemap->querySmart());
	getMidiPortRemap->setPortRemapDirection(pk::PortDirection::OUTPUT);
	m_pMidiPortRemapOut = std::dynamic_pointer_cast<RetSetMidiPortRemap>(
		getMidiPortRemap->querySmart());
}
