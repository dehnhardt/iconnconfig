#include "audioportparmwidget.h"
#include "../iconnconfigmain.h"
#include "centralwidget.h"
#include "portdisplayhelper.h"
#include "ui_audioportparmwidget.h"

#include <QMessageBox>

AudioPortParmWidget::AudioPortParmWidget(
	std::shared_ptr<RetSetAudioPortParm> retSetAudioPortParm, QWidget *parent)
	: QWidget(parent), ui(new Ui::AudioPortParmWidget),
	  m_pRetSetAudioPortParm(retSetAudioPortParm) {
	ui->setupUi(this);
	setData();
	m_pUpdateTimer = new QTimer();
	m_pUpdateTimer->setSingleShot(true);
	createConnections();
}

AudioPortParmWidget::~AudioPortParmWidget() {
	delete m_pUpdateTimer;
	delete ui;
}

void AudioPortParmWidget::setData() {
	this->ui->m_pEditPortId->setText(
		QString::number(m_pRetSetAudioPortParm->getPortId()));
	ui->m_pLblPortIcon->setPixmap(
		PortDisplayHelper::getAudioPortIcon(
			m_pRetSetAudioPortParm->getAudioPortType())
			.pixmap(QSize(30, 30)));
	ui->m_pEditPortNumber->setText(
		QString::number(m_pRetSetAudioPortParm->getDeviceSpecficPortNumer()));
	switch (m_pRetSetAudioPortParm->getAudioPortType()) {
	case APT_ANALOGUE:
		ui->m_pGbUSBDevice->setVisible(false);
		ui->m_pLblJackSpecificDeviceNumber->setVisible(false);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(false);
		break;
	case APT_USB_DEVICE:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(false);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(false);
		ui->m_pGbUSBDevice->setVisible(true);
		ui->m_pChbPCSupported->setChecked(
			m_pRetSetAudioPortParm->getPortSupportsPC());
		ui->m_pChbIOSSupported->setChecked(
			m_pRetSetAudioPortParm->getPortSupportsIOS());
		ui->m_pChbPcEnabled->setChecked(
			m_pRetSetAudioPortParm->getPortPCEnabled());
		ui->m_pChbIOSEnabled->setChecked(
			m_pRetSetAudioPortParm->getPortIOSEnabled());
		break;
	case APT_USB_HOST:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setText(QString::number(
			m_pRetSetAudioPortParm->getJackSpecificDeviceNumber()));
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	case APT_ETHERNET:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setText(QString::number(
			m_pRetSetAudioPortParm->getJackSpecificDeviceNumber()));
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	case APT_NONE:
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	}
	this->ui->m_pEditPortName->setText(
		m_pRetSetAudioPortParm->getPortName().c_str());
	this->ui->m_pEditPortName->setValidator(
		new QRegExpValidator(QRegExp("[0-9a-zA-Z -_]+")));
	this->ui->m_pEditPortName->setReadOnly(
		!m_pRetSetAudioPortParm->getPortNameWritable());
	this->ui->m_pEditPortName->setMaxLength(
		m_pRetSetAudioPortParm->getMaxPortNameLength());
	setCurrentAudioConfiguration();
}

void AudioPortParmWidget::setCurrentAudioConfiguration() {
	AudioPortConfiguration *audioPortConfiguration =
		m_pRetSetAudioPortParm->getCurrentAudioConfiguration();
	ui->m_pCbNumberInputChannels->clear();
	for (int i = audioPortConfiguration->minInputChannelsSupported;
		 i <= audioPortConfiguration->maxInputChannelsSupported; i++)
		ui->m_pCbNumberInputChannels->addItem(QString::number(i));
	ui->m_pCbNumberOutputChannels->clear();
	for (int i = audioPortConfiguration->minOutputChannelsSupported;
		 i <= audioPortConfiguration->maxOutputChannelsSupported; i++)
		ui->m_pCbNumberOutputChannels->addItem(QString::number(i));
	ui->m_pEditMaximumNumberOfPorts->setText(
		QString::number(audioPortConfiguration->maxAudioChannelsSupported));
	ui->m_pCbNumberInputChannels->setCurrentText(
		QString::number(m_pRetSetAudioPortParm->getInputChannels()));
	ui->m_pCbNumberOutputChannels->setCurrentText(
		QString::number(m_pRetSetAudioPortParm->getOutputChannels()));
	ui->m_pCbNumberInputChannels->setEnabled(
		!(audioPortConfiguration->minInputChannelsSupported ==
		  audioPortConfiguration->maxInputChannelsSupported));
	ui->m_pCbNumberOutputChannels->setEnabled(
		!(audioPortConfiguration->minOutputChannelsSupported ==
		  audioPortConfiguration->maxOutputChannelsSupported));
	ui->m_pEditAudioConfigString->setText(
		m_pRetSetAudioPortParm->getCurrentAudioConfigurationString().c_str());
}

void AudioPortParmWidget::createConnections() {
	CentralWidget *w = MioMain::getMainWin()->getCentralDeviceWidget();
	connect(w, &CentralWidget::changeAudioConfig, this,
			&AudioPortParmWidget::audioConfigurationChanged);

	connect(ui->m_pChbIOSEnabled, &QCheckBox::stateChanged, [=](int state) {
		m_pRetSetAudioPortParm->setPortIOSEnabled(state == 2);
		m_pUpdateTimer->start(1000);
	});
	connect(ui->m_pChbPcEnabled, &QCheckBox::stateChanged, [=](int state) {
		m_pRetSetAudioPortParm->setPortPCEnabled(state == 2);
		m_pUpdateTimer->start(1000);
	});
	connect(ui->m_pCbNumberInputChannels, &QComboBox::currentTextChanged,
			[=](QString text) {
				m_pRetSetAudioPortParm->setInputChannels(text.toInt());
				if (this->checkTotalNumberOfAudioChannels()) {
					m_pUpdateTimer->start(1000);
				} else {
					m_pUpdateTimer->stop();
				}
			});
	connect(ui->m_pCbNumberOutputChannels, &QComboBox::currentTextChanged,
			[=](QString text) {
				m_pRetSetAudioPortParm->setOutputChannels(text.toInt());
				if (this->checkTotalNumberOfAudioChannels()) {
					m_pUpdateTimer->start(1000);
				} else {
					m_pUpdateTimer->stop();
				}
			});
	connect(ui->m_pEditPortName, &QLineEdit::editingFinished, [=] {
		std::string portName = ui->m_pEditPortName->text().toStdString();
		m_pRetSetAudioPortParm->setPortName(portName);
		emit changePortName(portName);
		m_pUpdateTimer->start(1000);
	});

	connect(m_pUpdateTimer, &QTimer::timeout, [=] {
		m_pRetSetAudioPortParm->setDebug(false);
		m_pRetSetAudioPortParm->execute();
	});
}

bool AudioPortParmWidget::checkTotalNumberOfAudioChannels() {
	if (m_pRetSetAudioPortParm->getInputChannels() +
			m_pRetSetAudioPortParm->getOutputChannels() >
		m_pRetSetAudioPortParm->getCurrentAudioConfiguration()
			->maxAudioChannelsSupported) {
		QMessageBox::information(
			this, tr("Configuration Error"),
			tr("The total number of inpot channels and output channels "
			   "is bigger than the maximum of allowed channels"));
		return false;
	}
	return true;
}

void AudioPortParmWidget::audioConfigurationChanged() {
	setCurrentAudioConfiguration();
}
