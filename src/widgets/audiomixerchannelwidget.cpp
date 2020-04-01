#include "audiomixerchannelwidget.h"
#include "ui_audiochannelfeaturewidget.h"

#include "../sysex/retsetaudioportparm.h"

AudioMixerChannelWidget::AudioMixerChannelWidget(
	Device *device, AudioPortId portId, AudioChannelId channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget),
	  m_channelDirection(channelDirection), m_pDevice(device),
	  m_iPortId(portId), m_iMixerChannelId(channelNumber) {
	m_pUpdateTimer = new QTimer();
	m_pUpdateTimer->setInterval(10);
	m_pUpdateTimer->setSingleShot(true);
	m_iPortType = m_pDevice->getAudioPort(portId)->getAudioPortType();
	connect(m_pUpdateTimer, &QTimer::timeout, this,
			&AudioMixerChannelWidget::audioChannelValueChanged);
}

AudioMixerChannelWidget::AudioMixerChannelWidget(QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {}

AudioMixerChannelWidget::~AudioMixerChannelWidget() {}

QFrame *AudioMixerChannelWidget::getFirstConnectionFrame() {
	return this->ui->m_pFrmUpperMixerPanel;
}

QFrame *AudioMixerChannelWidget::getSecondConnectionFrame() {
	return this->ui->m_pFrmUpperMixerPanel2;
}

QToolButton *AudioMixerChannelWidget::getConnectionButton() {
	return m_pBtnSelectConnection;
}

QString AudioMixerChannelWidget::getChannelName() {
	return QString::number(m_iMixerChannelId);
}

int AudioMixerChannelWidget::getPanValue() {
	return this->ui->m_pSlidePan->value();
}

void AudioMixerChannelWidget::setPanValue(int value) {
	this->ui->m_pSlidePan->setValue(value);
}

int AudioMixerChannelWidget::calculateBalance(int panL, int panR) {
	int balance = (panL + panR) / 2;
	return balance;
}

int AudioMixerChannelWidget::calculatePanL(int balance) {
	int min = this->ui->m_pSlidePan->minimum();
	int panL = balance <= 0 ? min : min + 2 * balance;
	return panL;
}

int AudioMixerChannelWidget::calculatePanR(int balance) {
	int max = this->ui->m_pSlidePan->maximum();
	int panR = balance >= 0 ? max : max + 2 * balance;
	return panR;
}

void AudioMixerChannelWidget::initControls() {

	ui->m_pDial->setVisible(false);
	ui->m_plblTrimPan->setVisible(false);
	ui->m_pSlidePan->setVisible(false);
	ui->m_pTbHighImpedance->setVisible(false);
	ui->m_pTbPhantomPower->setVisible(false);
	ui->m_pTbMute->setVisible(false);
	ui->m_pTbSolo->setVisible(false);
	ui->m_pTbPfl->setVisible(false);
	ui->m_pTbInvert->setVisible(false);
	ui->m_pChbStereoLink->setVisible(false);
	ui->m_pFrmUpperMixerPanel2->setEnabled(false);

	ui->m_pTbMute->setColor(255, 0, 0);
	ui->m_pTbSolo->setColor(255, 188, 17);
	ui->m_pTbPfl->setColor(255, 138, 55);
	ui->m_pTbInvert->setColor(85, 85, 255);
	ui->m_pTbHighImpedance->setColor(255, 206, 20);
	ui->m_pTbPhantomPower->setColor(255, 0, 0);

	this->ui->m_pPBRight->setVisible(false);
	this->ui->m_pPBLeft->setRange(1, 8192);
	this->ui->m_pPBRight->setRange(1, 8192);
}

void AudioMixerChannelWidget::changeSoloStatus(bool enabled) {
	this->ui->m_pTbSolo->setChecked(enabled);
}

void AudioMixerChannelWidget::changePFLStatus(bool enabled) {
	this->ui->m_pTbPfl->setChecked(enabled);
}

void AudioMixerChannelWidget::changeInvertStatus(bool enabled) {
	this->ui->m_pTbInvert->setChecked(enabled);
}

void AudioMixerChannelWidget::changeMuteStatus(bool enabled) {
	this->ui->m_pTbMute->setChecked(enabled);
}

void AudioMixerChannelWidget::changeVolume(int volume) {
	this->ui->m_pSlideVolume->setValue(volume);
}

void AudioMixerChannelWidget::changePan(int pan) {
	this->ui->m_pDial->setValue(pan);
}

void AudioMixerChannelWidget::changeMeterVolume(unsigned int channel,
												int value) {
	if (channel == m_iMixerChannelId) {
		ui->m_pPBLeft->setValue(value);
	}
	if (m_bIsMaster && channel == m_iMixerChannelId + 1)
		ui->m_pPBRight->setValue(value);
}
