#include "audiomixerchannelwidget.h"
#include "ui_audiochannelfeaturewidget.h"

AudioMixerChannelWidget::AudioMixerChannelWidget(
	Device *device, unsigned int portId, unsigned int channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget),
	  m_channelDirection(channelDirection), m_pDevice(device),
	  m_iPortId(portId), m_iChannelNumber(channelNumber) {}

AudioMixerChannelWidget::AudioMixerChannelWidget(QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {}

AudioMixerChannelWidget::~AudioMixerChannelWidget() {}

void AudioMixerChannelWidget::initControls() {
	ui->m_pTbHighImpedance->setVisible(false);
	ui->m_pTbPhantomPower->setVisible(false);
	ui->m_pTbMute->setVisible(false);
	ui->m_pTbSolo->setVisible(false);
	ui->m_pTbPfl->setVisible(false);
	ui->m_pTbInvert->setVisible(false);

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
	this->ui->m_pDialTrim->setValue(pan);
}

void AudioMixerChannelWidget::changeMeterVolume(unsigned int channel,
												int value) {
	if (channel == m_iChannelNumber) {
		ui->m_pPBLeft->setValue(value);
	}
	if (m_bIsMaster && channel == m_iChannelNumber + 1)
		ui->m_pPBRight->setValue(value);
}
