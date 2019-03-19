#include "audiochannelfeaturewidget.h"
#include "../sysex/communicationexception.h"
#include "../sysex/protocolexception.h"
#include "ui_audiochannelfeaturewidget.h"

AudioChannelFeatureWidget::AudioChannelFeatureWidget(
	RetSetAudioControlDetail *retSetAudioControlDetail, QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {
	ui->setupUi(this);
	setRetSetAudioControlDetail(retSetAudioControlDetail);
	if (m_pAudioControlDetail->hasFeatures()) {
		queryValues();
		if (m_pRetSetAudioControlDetailValue)
			setValues(this->m_pRetSetAudioControlDetailValue);
		m_pUpdateTimer = new QTimer();
		m_pUpdateTimer->setSingleShot(true);
	}
	createConnections();
}

AudioChannelFeatureWidget::~AudioChannelFeatureWidget() {
	delete ui;
	if (m_pUpdateTimer)
		delete m_pUpdateTimer;
	/*if (m_pAudioControlDetail)
		delete m_pAudioControlDetail;
	if (m_pRetSetAudioControlDetailValue)
		delete m_pRetSetAudioControlDetailValue;*/
}

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

		/*int range = retSetAudioControlDetail->getMaxTrimValue() -
					retSetAudioControlDetail->getMinTrimValue();*/

		this->ui->m_pDialTrim->setMinimum(
			retSetAudioControlDetail->getMinTrimValue());
		this->ui->m_pDialTrim->setMaximum(
			retSetAudioControlDetail->getMaxTrimValue());
		connect(this->ui->m_pDialTrim, &QSlider::valueChanged,
				[](int val) { std::cout << std::dec << val << std::endl; });
	} else {
		this->ui->m_pFrmVolume->setVisible(false);
	}
}

void AudioChannelFeatureWidget::queryValues() {
	GetAudioControlDetailValue *getAudioControlDetailValue = nullptr;
	getAudioControlDetailValue =
		new GetAudioControlDetailValue(m_pAudioControlDetail->getDevice());
	getAudioControlDetailValue->setPortId(m_pAudioControlDetail->getPortId());
	getAudioControlDetailValue->setControllerNumber(
		m_pAudioControlDetail->getControllerNumber());
	getAudioControlDetailValue->setDetailNumber(
		m_pAudioControlDetail->getDetailNumber());
	try {
		m_pRetSetAudioControlDetailValue =
			dynamic_cast<RetSetAudioControlDetailValue *>(
				getAudioControlDetailValue->query());
	} catch (ProtocolException protocolException) {
		std::cerr << protocolException.getErrorMessage() << std::endl;
	} catch (std::out_of_range oor) {
		std::cerr << oor.what() << std::endl;
	}
}

void AudioChannelFeatureWidget::setValues(
	RetSetAudioControlDetailValue *retSetAudioControlDetailValue) {
	if (m_pAudioControlDetail->hasVolumeControl() &&
		retSetAudioControlDetailValue->hasVolumeControl()) {
		ui->m_pSlideVolume->setValue(
			retSetAudioControlDetailValue->getVolume());
		ui->m_pDialTrim->setValue(retSetAudioControlDetailValue->getTrim());
		retSetAudioControlDetailValue->setHasVolumeControl(false);
		connect(this->ui->m_pSlideVolume, &QSlider::valueChanged,
				[=](int value) {
					this->m_pRetSetAudioControlDetailValue->setVolume(value);
					this->m_pUpdateTimer->start(10);
				});
		connect(this->ui->m_pDialTrim, &QDial::valueChanged, [=](int value) {
			this->m_pRetSetAudioControlDetailValue->setTrim(value);
			this->m_pUpdateTimer->start(10);
		});
	}
	if (m_pAudioControlDetail->hasMuteControl() &&
		retSetAudioControlDetailValue->hasMuteControl()) {
		ui->m_pChbMute->setChecked(retSetAudioControlDetailValue->getMute());
		retSetAudioControlDetailValue->setHasMuteControl(false);
		connect(this->ui->m_pChbMute, &QCheckBox::stateChanged, [=](int state) {
			this->m_pRetSetAudioControlDetailValue->setMute(state == 2);
			this->m_pUpdateTimer->start(10);
		});
	}
	if (retSetAudioControlDetailValue->hasPhantomPowerControl()) {
		ui->m_pChbPhantomPower->setChecked(
			retSetAudioControlDetailValue->getPhantomPower());
		retSetAudioControlDetailValue->setHasPhantomPowerControl(false);
		connect(ui->m_pChbPhantomPower, &QCheckBox::stateChanged,
				[=](int state) {
					this->m_pRetSetAudioControlDetailValue->setPhantomPower(
						state == 2);
					this->m_pUpdateTimer->start(10);
				});
	}
	if (retSetAudioControlDetailValue->hasHighImpendanceControl()) {
		ui->m_pChbHighImpedance->setChecked(
			retSetAudioControlDetailValue->getHigImpedance());
		retSetAudioControlDetailValue->setHasHighImpendanceControl(false);
	}
	if (retSetAudioControlDetailValue->hasStereoLinkControl()) {
		ui->m_pChbStereoLink->setChecked(
			retSetAudioControlDetailValue->getSteroLink());
		retSetAudioControlDetailValue->setHasStereoLinkControl(false);
	}
}

void AudioChannelFeatureWidget::createConnections() {
	connect(m_pUpdateTimer, &QTimer::timeout, this,
			&AudioChannelFeatureWidget::audioChannelValueChanged);
}

void AudioChannelFeatureWidget::audioChannelValueChanged() {
	try {
		m_pRetSetAudioControlDetailValue->setDebug(true);
		m_pRetSetAudioControlDetailValue->execute();
		m_pRetSetAudioControlDetailValue->setDebug(false);
	} catch (CommunicationException e) {
		std::cerr << e.getErrorMessage() << std::endl;
	}
	m_pRetSetAudioControlDetailValue->reset();
}
