#include "portinfowidget.h"
#include "portdisplayhelper.h"
#include "ui_portinfowidget.h"
#include <QIcon>

PortInfoWidget::PortInfoWidget(RetSetMidiPortInfo *portInfo, QWidget *parent)
	: QWidget(parent), ui(new Ui::PortInfoWidget), m_pMidiPortInfo(portInfo) {
	ui->setupUi(this);
	this->ui->m_pLblPortId->setNum(
		static_cast<int>(m_pMidiPortInfo->getPortId()));
	ui->m_pLblPortIcon->setPixmap(
		PortDisplayHelper::getPortIcon(m_pMidiPortInfo->getPortType())
			.pixmap(QSize(30, 30)));
	this->ui->m_pLblPortNr->setNum(
		static_cast<int>(m_pMidiPortInfo->getJackNumber()));
	this->ui->m_pTxtPortName->setText(m_pMidiPortInfo->getPortName().c_str());
	this->ui->m_pTxtPortName->setReadOnly(
		!m_pMidiPortInfo->getPortNameWritable());
	ui->m_pCbInputEnabled->setCheckState(
		m_pMidiPortInfo->getInputEnabled() ? Qt::Checked : Qt::Unchecked);
	ui->m_pCbOutputEnabled->setCheckState(
		m_pMidiPortInfo->getOutputEnabled() ? Qt::Checked : Qt::Unchecked);
	m_pUpdateTimer = new QTimer();
	m_pUpdateTimer->setSingleShot(true);
	createConnections();
}

PortInfoWidget::~PortInfoWidget() { delete ui; }

void PortInfoWidget::portNameChanged() {

	m_pMidiPortInfo->setPortName(ui->m_pTxtPortName->text().toStdString());
	m_pUpdateTimer->start(1000);
}

void PortInfoWidget::inputStateChanged(int state) {
	m_pMidiPortInfo->setInputEnabled(state == Qt::Checked);
	m_pUpdateTimer->start(1000);
}

void PortInfoWidget::outputStateChanged(int state) {
	m_pMidiPortInfo->setoutputEnabled(state == Qt::Checked);
	m_pUpdateTimer->start(1000);
}

void PortInfoWidget::timerElapsed() {
	this->m_pMidiPortInfo->setDebug(true);
	if (this->m_pMidiPortInfo->execute() == 0) {
		emit changePortName(m_pMidiPortInfo->getPortName());
	}
	this->m_pMidiPortInfo->setDebug(false);
}

void PortInfoWidget::createConnections() {
	connect(ui->m_pTxtPortName, &QLineEdit::editingFinished, this,
			&PortInfoWidget::portNameChanged);
	connect(ui->m_pCbInputEnabled, &QCheckBox::stateChanged, this,
			&PortInfoWidget::inputStateChanged);
	connect(ui->m_pCbOutputEnabled, &QCheckBox::stateChanged, this,
			&PortInfoWidget::outputStateChanged);
	connect(m_pUpdateTimer, &QTimer::timeout, this,
			&PortInfoWidget::timerElapsed);
}
