#include "audioportparmwidget.h"
#include "portdisplayhelper.h"
#include "ui_audioportparmwidget.h"

AudioPortParmWidget::AudioPortParmWidget(
	RetSetAudioPortParm *retSetAudioPortParm, QWidget *parent)
	: QWidget(parent), ui(new Ui::AudioPortParmWidget),
	  m_pRetSetAudioPortParm(retSetAudioPortParm) {
	ui->setupUi(this);
	setData();
}

AudioPortParmWidget::~AudioPortParmWidget() { delete ui; }

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
	for (int i = audioPortConfiguration->minInputChannelsSupported;
		 i <= audioPortConfiguration->maxInputChannelsSupported; i++)
		ui->m_pCbNumberInputChannels->addItem(QString::number(i));
	for (int i = audioPortConfiguration->minOutputChannelsSupported;
		 i <= audioPortConfiguration->maxOutputChannelsSupported; i++)
		ui->m_pCbNumberOutputChannels->addItem(QString::number(i));
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

void AudioPortParmWidget::audioConfigurationChanged() {
	setCurrentAudioConfiguration();
}
