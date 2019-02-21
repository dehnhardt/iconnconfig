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

void PortFilterWidget::midiPortFilterUpdated(PortFilterDirection direction) {
	switch (direction) {
	case PortFilterDirection::INPUT:
		m_pUpdateTimerInFilter->start(1000);
		break;
	case PortFilterDirection::OUTPUT:
		m_pUpdateTimerOutFilter->start(1000);
		break;
	}
}

void PortFilterWidget::updateInFilter() {
	/*MIDI::printMessage(m_pMidiPortFilterIn->m_pGetMessageData());
	std::cout << std::endl;*/
	m_pMidiPortFilterIn->execute();
}

void PortFilterWidget::updateOutFilter() { m_pMidiPortFilterIn->execute(); }

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
	connect(m_pUpdateTimerInFilter, &QTimer::timeout, this,
			&PortFilterWidget::updateInFilter);
}

void PortFilterWidget::createWidgets() {
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);
	m_pFilterDirectionTabWidget = new QTabWidget(this);
	m_pFilterDirectionTabWidget->setTabPosition(QTabWidget::West);
	m_pInputFilterWidget = new PortFilterSettingsWidget(this);
	m_pOutputFilterWidget = new PortFilterSettingsWidget(this);
	m_pFilterDirectionTabWidget->addTab(m_pInputFilterWidget,
										tr("Input Filter"));
	m_pFilterDirectionTabWidget->addTab(m_pOutputFilterWidget,
										tr("Output Filter"));
	gridLayout->addWidget(m_pFilterDirectionTabWidget);
}

void PortFilterWidget::retrieveData() {
	GetMidiPortFilter *getMidiPortFilter = new GetMidiPortFilter(device);
	getMidiPortFilter->setDebug(true);
	getMidiPortFilter->setPortNumer(static_cast<unsigned int>(portNumber));
	getMidiPortFilter->setPortFilterDirection(PortFilterDirection::INPUT);
	getMidiPortFilter->setDebug(true);
	getMidiPortFilter->setCmdflags(0x40);
	m_pMidiPortFilterIn =
		static_cast<RetSetMidiPortFilter *>(getMidiPortFilter->query());
	getMidiPortFilter->setPortFilterDirection(PortFilterDirection::OUTPUT);
	m_pMidiPortFilterOut =
		static_cast<RetSetMidiPortFilter *>(getMidiPortFilter->query());
}
