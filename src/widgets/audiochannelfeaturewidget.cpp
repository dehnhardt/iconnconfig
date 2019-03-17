#include "audiochannelfeaturewidget.h"
#include "ui_audiochannelfeaturewidget.h"

AudioChannelFeatureWidget::AudioChannelFeatureWidget(
	RetSetAudioControlDetail *retSetAudioControlDetail, QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {
	ui->setupUi(this);
	setRetSetAudioControlDetail(retSetAudioControlDetail);
}

AudioChannelFeatureWidget::~AudioChannelFeatureWidget() { delete ui; }

void AudioChannelFeatureWidget::setRetSetAudioControlDetail(
	RetSetAudioControlDetail *retSetAudioControlDetail) {
	this->m_pAudioControlDetail = retSetAudioControlDetail;
	this->ui->m_pLblChannelName->setText(
		retSetAudioControlDetail->getChannelName().c_str());
	this->ui->m_pChbMute->setVisible(
		retSetAudioControlDetail->hasMuteControl());
	this->ui->m_pChbMute->setEnabled(
		retSetAudioControlDetail->getMuteControlEditable());
	this->ui->m_pChbStereoLink->setVisible(
		retSetAudioControlDetail->hasStereoLinkControl());
	this->ui->m_pChbStereoLink->setEnabled(
		retSetAudioControlDetail->getStereoLinkControlEditable());
	this->ui->m_pChbPhantomPower->setVisible(
		retSetAudioControlDetail->hasPhantomPowerControl());
	this->ui->m_pChbPhantomPower->setEnabled(
		retSetAudioControlDetail->getPhantomPowerControlEditable());
	this->ui->m_pChbHighImpedance->setVisible(
		retSetAudioControlDetail->hasHighImpendanceControl());
	this->ui->m_pChbHighImpedance->setEnabled(
		retSetAudioControlDetail->getHighImpendanceControlEditable());
}
