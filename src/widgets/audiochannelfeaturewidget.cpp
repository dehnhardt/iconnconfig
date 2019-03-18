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
	if (retSetAudioControlDetail->hasMuteControl()) {
		this->ui->m_pChbMute->setVisible(true);
		this->ui->m_pChbMute->setEnabled(
			retSetAudioControlDetail->getMuteControlEditable());
	} else {
		this->ui->m_pChbMute->setVisible(false);
	}
	if (retSetAudioControlDetail->hasStereoLinkControl()) {
		this->ui->m_pChbStereoLink->setVisible(true);
		this->ui->m_pChbStereoLink->setEnabled(
			retSetAudioControlDetail->getStereoLinkControlEditable());
	} else {
		this->ui->m_pChbStereoLink->setVisible(false);
	}
	if (retSetAudioControlDetail->hasPhantomPowerControl()) {
		this->ui->m_pChbPhantomPower->setVisible(true);
		this->ui->m_pChbPhantomPower->setEnabled(
			retSetAudioControlDetail->getPhantomPowerControlEditable());
	} else {
		this->ui->m_pChbPhantomPower->setVisible(false);
	}
	if (retSetAudioControlDetail->hasHighImpendanceControl()) {
		this->ui->m_pChbHighImpedance->setVisible(true);
		this->ui->m_pChbHighImpedance->setEnabled(
			retSetAudioControlDetail->getHighImpendanceControlEditable());
	} else {
		this->ui->m_pChbHighImpedance->setVisible(false);
	}
	if (retSetAudioControlDetail->hasVolumeControl()) {
		this->ui->m_pFrmVolume->setVisible(true);
		this->ui->m_pFrmVolume->setEnabled(
			retSetAudioControlDetail->getVolumeControlEditable());
		this->ui->m_pSlideVolume->setMinimum(
			retSetAudioControlDetail->getMinVolumeValue());
		this->ui->m_pSlideVolume->setMaximum(
			retSetAudioControlDetail->getMaxVolumeValue());
		this->ui->m_pSlideVolume->setTickInterval(
			retSetAudioControlDetail->getVolumeResolution() * 10);

		connect(this->ui->m_pSlideVolume, &QSlider::valueChanged,
				[](int val) { std::cout << std::dec << val << std::endl; });

		int range = retSetAudioControlDetail->getMaxTrimValue() -
					retSetAudioControlDetail->getMinTrimValue();

		this->ui->m_pDialTrim->setMinimum(0);
		this->ui->m_pDialTrim->setMaximum(10);
		connect(this->ui->m_pDialTrim, &QSlider::valueChanged,
				[](int val) { std::cout << std::dec << val << std::endl; });
	} else {
		this->ui->m_pFrmVolume->setVisible(false);
	}
}
