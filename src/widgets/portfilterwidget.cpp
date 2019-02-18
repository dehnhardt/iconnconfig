#include "portfilterwidget.h"

#include <QLabel>

#include <src/sysex/getmidiportfilter.h>

PortFilterWidget::PortFilterWidget(Device *device, unsigned int portNumber,
								   QWidget *parent)
	: QWidget(parent), device(device), portNumber(portNumber) {

	createWidgets();
	retrieveData();
	loadData();

	/*buttonLines = new std::vector<std::vector<PortButton *> *>;
	updateTimer = new QTimer(this);
	updateTimer->setSingleShot(true);
	createSignalMapper();
	createWidgets();
	loadData();
	connect(lineButtonSignalMapper, SIGNAL(mapped(QObject *)), this,
			SLOT(lineButtonClicked(QObject *)));

	connect(portButtonSignalMapper, SIGNAL(mapped(QObject *)), this,
			SLOT(portButtonClicked(QObject *)));

	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateRouting()));*/
}

PortFilterWidget::~PortFilterWidget() {}

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
	getMidiPortFilter->setPortNumer(portNumber);
	getMidiPortFilter->setPortFilterDirection(PortFilterDirection::INPUT);
	getMidiPortFilter->setDebug(true);
	getMidiPortFilter->setCmdflags(0x40);
	m_pMidiPortFilterIn =
		static_cast<RetSetMidiPortFilter *>(getMidiPortFilter->query());
	getMidiPortFilter->setPortFilterDirection(PortFilterDirection::OUTPUT);
	m_pMidiPortFilterOut =
		static_cast<RetSetMidiPortFilter *>(getMidiPortFilter->query());
}
