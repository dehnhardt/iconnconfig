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
	m_pRegExpValidator = new QRegExpValidator(QRegExp("[0-9a-zA-Z -_]+"));
	setData();
	m_pUpdateTimer = new QTimer();
	m_pUpdateTimer->setSingleShot(true);
	createConnections();
}

AudioPortParmWidget::~AudioPortParmWidget() {
	delete m_pUpdateTimer;
	delete m_pRegExpValidator;
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
		QString::number(m_pRetSetAudioPortParm->getDeviceSpecficPortNumber()));
	switch (m_pRetSetAudioPortParm->getAudioPortType()) {
	case pk::AudioPortType::APT_ANALOGUE:
		ui->m_pGbUSBDevice->setVisible(false);
		ui->m_pLblJackSpecificDeviceNumber->setVisible(false);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(false);
		break;
	case pk::AudioPortType::APT_USB_DEVICE:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(false);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(false);
		ui->m_pGbUSBDevice->setVisible(true);
		if (m_pRetSetAudioPortParm->getPortSupportsPC()) {
			ui->m_pChbPcEnabled->setVisible(true);
			ui->m_pChbPCSupported->setVisible(true);
			ui->m_pChbPCConnected->setVisible(true);

			ui->m_pChbPCSupported->setChecked(true);
			ui->m_pChbPcEnabled->setChecked(
				m_pRetSetAudioPortParm->getPortPCEnabled());
		} else {
			ui->m_pChbPcEnabled->setVisible(false);
			ui->m_pChbPCSupported->setVisible(false);
			ui->m_pChbPCConnected->setVisible(false);
		}
		if (m_pRetSetAudioPortParm->getPortSupportsIOS()) {
			ui->m_pChbIOSEnabled->setVisible(true);
			ui->m_pChbIOSSupported->setVisible(true);
			ui->m_pChbIOSConnected->setVisible(true);
			ui->m_pLblHostName->setVisible(true);
			ui->m_pEditHostName->setVisible(true);

			ui->m_pChbIOSSupported->setChecked(true);
			ui->m_pChbIOSEnabled->setChecked(
				m_pRetSetAudioPortParm->getPortIOSEnabled());
		} else {
			ui->m_pChbIOSEnabled->setVisible(false);
			ui->m_pChbIOSSupported->setVisible(false);
			ui->m_pChbIOSConnected->setVisible(false);
			ui->m_pLblHostName->setVisible(false);
			ui->m_pEditHostName->setVisible(false);
		}
		ui->m_pChbIOSSupported->setChecked(
			m_pRetSetAudioPortParm->getPortSupportsIOS());
		ui->m_pChbIOSEnabled->setChecked(
			m_pRetSetAudioPortParm->getPortIOSEnabled());
		break;
	case pk::AudioPortType::APT_USB_HOST:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setText(QString::number(
			m_pRetSetAudioPortParm->getJackSpecificDeviceNumber()));
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	case pk::AudioPortType::APT_ETHERNET:
		ui->m_pLblJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setVisible(true);
		ui->m_pEditJackSpecificDeviceNumber->setText(QString::number(
			m_pRetSetAudioPortParm->getJackSpecificDeviceNumber()));
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	case pk::AudioPortType::APT_NONE:
		ui->m_pGbUSBDevice->setVisible(false);
		break;
	}
	this->ui->m_pEditPortName->setText(
		m_pRetSetAudioPortParm->getPortName().c_str());
	this->ui->m_pEditPortName->setValidator(m_pRegExpValidator);
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
	for (unsigned int i = audioPortConfiguration->minInputChannelsSupported;
		 i <= audioPortConfiguration->maxInputChannelsSupported; i++)
		ui->m_pCbNumberInputChannels->addItem(QString::number(i));
	ui->m_pCbNumberOutputChannels->clear();
	for (unsigned int i = audioPortConfiguration->minOutputChannelsSupported;
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

void AudioPortParmWidget::setAudioDeviceParm(
	std::shared_ptr<RetSetAudioDeviceParm> retSetAudioDeviceParm) {
	this->m_pRetSetAudioDeviceParm = retSetAudioDeviceParm;
	if (m_pRetSetAudioDeviceParm->getUsbDeviceHost() == pk::UsbDeviceHost::MAC_PC) {
		this->ui->m_pChbPCConnected->setChecked(true);
		this->ui->m_pChbIOSConnected->setChecked(false);
	} else if (m_pRetSetAudioDeviceParm->getUsbDeviceHost() ==
			   pk::UsbDeviceHost::IOS) {
		this->ui->m_pChbPCConnected->setChecked(false);
		this->ui->m_pChbIOSConnected->setChecked(true);
	} else {
		this->ui->m_pChbPCConnected->setChecked(false);
		this->ui->m_pChbIOSConnected->setChecked(false);
	}
	if (m_pRetSetAudioDeviceParm->hastHostName())
		this->ui->m_pEditHostName->setText(
			m_pRetSetAudioDeviceParm->getHostName().c_str());
	else
		this->ui->m_pEditHostName->setText("");
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
				m_pRetSetAudioPortParm->setInputChannels(text.toUInt());
				if (this->checkTotalNumberOfAudioChannels()) {
					m_pUpdateTimer->start(1000);
				} else {
					m_pUpdateTimer->stop();
				}
			});
	connect(ui->m_pCbNumberOutputChannels, &QComboBox::currentTextChanged,
			[=](QString text) {
				m_pRetSetAudioPortParm->setOutputChannels(text.toUInt());
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
			tr("The total number of input and output channels "
			   "is greater than the maximum of allowed channels"));
		return false;
	}
	return true;
}

void AudioPortParmWidget::audioConfigurationChanged(
	__attribute__((unused)) unsigned int currentAudioConfigurationNumber) {
	setCurrentAudioConfiguration();
}
