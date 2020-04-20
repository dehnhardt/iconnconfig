#include "portfilterwidget.h"

#include <QLabel>

#include <src/sysex/getmidiportfilter.h>

PortFilterWidget::PortFilterWidget(Device *device, int portNumber,
								   QWidget *parent)
	: QWidget(parent), device(device), portNumber(portNumber) {
	m_pUpdateTimerInFilter = new QTimer(this);
	m_pUpdateTimerOutFilter = new QTimer(this);
	m_pUpdateTimerInFilter->setSingleShot(true);
	m_pUpdateTimerOutFilter->setSingleShot(true);

	createWidgets();
	retrieveData();
	loadData();
	createConnections();
}

PortFilterWidget::~PortFilterWidget() {
	delete m_pUpdateTimerInFilter;
	delete m_pUpdateTimerOutFilter;
}

void PortFilterWidget::midiPortFilterUpdated(pk::PortDirection direction) {
	switch (direction) {
	case pk::PortDirection::INPUT:
		m_pUpdateTimerInFilter->start(1000);
		break;
	case pk::PortDirection::OUTPUT:
		m_pUpdateTimerOutFilter->start(1000);
		break;
	default:
		break;
	}
}

void PortFilterWidget::updateInFilter() {
	std::cout << "updateInFilter" << std::endl;
	m_pMidiPortFilterIn->execute();
}

void PortFilterWidget::updateOutFilter() {
	std::cout << "updateOutFilter" << std::endl;
	m_pMidiPortFilterOut->execute();
}

void PortFilterWidget::loadData() {
	// Input Filter
	m_pInputFilterWidget->setMIDISystemMessagesFilter(
		m_pMidiPortFilterIn->getMidiPortFilter()->midiSystemMessagesFilter);
	m_pInputFilterWidget->setMidiControllerFilter(
		m_pMidiPortFilterIn->getMidiPortFilter()->midiControllerFilter);
	m_pInputFilterWidget->setMidiChannelMessagesFilter(
		m_pMidiPortFilterIn->getMidiPortFilter()->midiChannelMessagesFilter);

	// Output Filter
	m_pOutputFilterWidget->setMIDISystemMessagesFilter(
		m_pMidiPortFilterOut->getMidiPortFilter()->midiSystemMessagesFilter);
	m_pOutputFilterWidget->setMidiControllerFilter(
		m_pMidiPortFilterOut->getMidiPortFilter()->midiControllerFilter);
	m_pOutputFilterWidget->setMidiChannelMessagesFilter(
		m_pMidiPortFilterOut->getMidiPortFilter()->midiChannelMessagesFilter);
}

void PortFilterWidget::createConnections() {
	connect(m_pInputFilterWidget, &PortFilterSettingsWidget::filterDataChanged,
			this, &PortFilterWidget::midiPortFilterUpdated);
	connect(m_pOutputFilterWidget, &PortFilterSettingsWidget::filterDataChanged,
			this, &PortFilterWidget::midiPortFilterUpdated);
	connect(m_pUpdateTimerInFilter, &QTimer::timeout, this,
			&PortFilterWidget::updateInFilter);
	connect(m_pUpdateTimerOutFilter, &QTimer::timeout, this,
			&PortFilterWidget::updateOutFilter);
}

void PortFilterWidget::createWidgets() {
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);
	m_pFilterDirectionTabWidget = new QTabWidget(this);
	m_pFilterDirectionTabWidget->setTabPosition(QTabWidget::West);
	m_pInputFilterWidget =
		new PortFilterSettingsWidget(pk::PortDirection::INPUT, this);
	m_pOutputFilterWidget =
		new PortFilterSettingsWidget(pk::PortDirection::OUTPUT, this);
	m_pFilterDirectionTabWidget->addTab(m_pInputFilterWidget,
										tr("Input Filter"));
	m_pFilterDirectionTabWidget->addTab(m_pOutputFilterWidget,
										tr("Output Filter"));
	gridLayout->addWidget(m_pFilterDirectionTabWidget);
}

void PortFilterWidget::retrieveData() {
	std::shared_ptr<GetMidiPortFilter> getMidiPortFilter =
		std::make_shared<GetMidiPortFilter>(device);
	getMidiPortFilter->setDebug(false);
	getMidiPortFilter->setPortNumer(static_cast<unsigned int>(portNumber));
	getMidiPortFilter->setPortFilterDirection(pk::PortDirection::INPUT);
	getMidiPortFilter->setDebug(false);
	getMidiPortFilter->setCmdflags(0x40);
	m_pMidiPortFilterIn = std::dynamic_pointer_cast<RetSetMidiPortFilter>(
		getMidiPortFilter->querySmart());
	getMidiPortFilter->setPortFilterDirection(pk::PortDirection::OUTPUT);
	m_pMidiPortFilterOut = std::dynamic_pointer_cast<RetSetMidiPortFilter>(
		getMidiPortFilter->querySmart());
}
