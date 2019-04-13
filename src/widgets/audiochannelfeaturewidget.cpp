#include "audiochannelfeaturewidget.h"
#include "../sysex/communicationexception.h"
#include "../sysex/protocolexception.h"
#include "ui_audiochannelfeaturewidget.h"

AudioChannelFeatureWidget::AudioChannelFeatureWidget(
	std::shared_ptr<RetSetAudioControlDetail> retSetAudioControlDetail,
	QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {
	ui->setupUi(this);
	setStyleSheet("QProgressBar::chunk{margin: 0.5px; "
				  "background-color: #11CC11; "
				  "height: 10px; } ");
	this->ui->m_pPBRight->setVisible(false);
	this->ui->m_pPBLeft->setRange(1, 8192);
	this->ui->m_pPBRight->setRange(1, 8192);

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
}

void AudioChannelFeatureWidget::setMaster(bool isMaster, QString channel2Name) {
	this->ui->m_pPBRight->setVisible(isMaster);
	m_bIsMaster = isMaster;
	if (isMaster) {
		QString name = this->m_pAudioControlDetail->getChannelName().c_str();
		name.append(" / ");
		name.append(channel2Name);
		ui->m_pLblChannelName->setText(name);
	} else {
		ui->m_pLblChannelName->setText(
			this->m_pAudioControlDetail->getChannelName().c_str());
	}
}

void AudioChannelFeatureWidget::setRetSetAudioControlDetail(
	std::shared_ptr<RetSetAudioControlDetail> retSetAudioControlDetail) {
	this->m_pAudioControlDetail = retSetAudioControlDetail;
	this->m_iDetailId = retSetAudioControlDetail->getDetailNumber();
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
		this->ui->m_pSlideVolume->setDebug(false);
		this->ui->m_pSlideVolume->setScaleType(PKSlider::ScaleType::LOGARITHM);
		this->ui->m_pSlideVolume->setResulution(
			retSetAudioControlDetail->getVolumeResolution());
		this->ui->m_pSlideVolume->setMinimum(
			retSetAudioControlDetail->getMinVolumeValue());
		this->ui->m_pSlideVolume->setMaximum(
			retSetAudioControlDetail->getMaxVolumeValue());
		this->ui->m_pSlideVolume->setTickInterval(1.0f / 256.0f);

		std::cout << "Setting up Slider ("
				  << retSetAudioControlDetail->getDetailNumber() << ", "
				  << retSetAudioControlDetail->getChannelName()
				  << "): minValue "
				  << retSetAudioControlDetail->getMinVolumeValue()
				  << " maxValue "
				  << retSetAudioControlDetail->getMaxVolumeValue() << std::endl;

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
	std::unique_ptr<GetAudioControlDetailValue> getAudioControlDetailValue =
		std::make_unique<GetAudioControlDetailValue>(
			m_pAudioControlDetail->getDevice());
	getAudioControlDetailValue->setPortId(m_pAudioControlDetail->getPortId());
	getAudioControlDetailValue->setControllerNumber(
		m_pAudioControlDetail->getControllerNumber());
	getAudioControlDetailValue->setDetailNumber(
		m_pAudioControlDetail->getDetailNumber());
	try {
		m_pRetSetAudioControlDetailValue =
			std::dynamic_pointer_cast<RetSetAudioControlDetailValue>(
				getAudioControlDetailValue->querySmart());
	} catch (ProtocolException protocolException) {
		std::cerr << protocolException.getErrorMessage() << std::endl;
	} catch (std::out_of_range oor) {
		std::cerr << oor.what() << std::endl;
	}
}

void AudioChannelFeatureWidget::setValues(
	std::shared_ptr<RetSetAudioControlDetailValue>
		retSetAudioControlDetailValue) {
	if (m_pAudioControlDetail->hasVolumeControl() &&
		retSetAudioControlDetailValue->hasVolumeControl()) {
		ui->m_pSlideVolume->setValue(
			retSetAudioControlDetailValue->getVolume());
		ui->m_pDialTrim->setValue(retSetAudioControlDetailValue->getTrim());
		retSetAudioControlDetailValue->setHasVolumeControl(false);
		if (m_pAudioControlDetail->getVolumeControlEditable()) {
			connect(this->ui->m_pSlideVolume, &PKSlider::valueChanged,
					[=](int value) {
						this->m_pRetSetAudioControlDetailValue->setVolume(
							value);
						this->m_pUpdateTimer->start(10);
						emit this->volumeChanged(value);
					});
			connect(this->ui->m_pDialTrim, &QDial::valueChanged,
					[=](int value) {
						this->m_pRetSetAudioControlDetailValue->setTrim(value);
						this->m_pUpdateTimer->start(10);
						emit this->trimChanged(value);
					});
		}
	}
	if (m_pAudioControlDetail->hasMuteControl() &&
		retSetAudioControlDetailValue->hasMuteControl()) {
		ui->m_pChbMute->setChecked(retSetAudioControlDetailValue->getMute());
		retSetAudioControlDetailValue->setHasMuteControl(false);
		if (m_pAudioControlDetail->getMuteControlEditable()) {
			connect(
				this->ui->m_pChbMute, &QCheckBox::stateChanged, [=](int state) {
					this->m_pRetSetAudioControlDetailValue->setMute(state == 2);
					this->m_pUpdateTimer->start(10);
					emit this->muteStatusChanged(state == 2);
				});
		}
	}
	if (retSetAudioControlDetailValue->hasPhantomPowerControl()) {
		ui->m_pChbPhantomPower->setChecked(
			retSetAudioControlDetailValue->getPhantomPower());
		retSetAudioControlDetailValue->setHasPhantomPowerControl(false);
		if (m_pAudioControlDetail->getPhantomPowerControlEditable()) {
			connect(ui->m_pChbPhantomPower, &QCheckBox::stateChanged,
					[=](int state) {
						this->m_pRetSetAudioControlDetailValue->setPhantomPower(
							state == 2);
						this->m_pUpdateTimer->start(10);
						emit this->phantomPowerStatusChanged(state == 2);
					});
		}
	}
	if (retSetAudioControlDetailValue->hasHighImpendanceControl()) {
		ui->m_pChbHighImpedance->setChecked(
			retSetAudioControlDetailValue->getHigImpedance());
		retSetAudioControlDetailValue->setHasHighImpendanceControl(false);
		if (m_pAudioControlDetail->getHighImpendanceControlEditable()) {
			connect(ui->m_pChbHighImpedance, &QCheckBox::stateChanged,
					[=](int state) {
						this->m_pRetSetAudioControlDetailValue->setHigImpedance(
							state == 2);
						this->m_pUpdateTimer->start(10);
						emit this->highImpedanceStatusChanged(state == 2);
					});
		}
	}
	if (retSetAudioControlDetailValue->hasStereoLinkControl()) {
		ui->m_pChbStereoLink->setChecked(
			retSetAudioControlDetailValue->getSteroLink());
		retSetAudioControlDetailValue->setHasStereoLinkControl(false);
		if (m_pAudioControlDetail->getStereoLinkControlEditable()) {
			connect(ui->m_pChbStereoLink, &QToolButton::clicked,
					[=](bool clicked) {
						this->m_pRetSetAudioControlDetailValue->setSteroLink(
							clicked);
						this->m_pUpdateTimer->start(10);
						emit this->linkStatusChanged(m_iDetailId, clicked);
					});
		}
	}
}

void AudioChannelFeatureWidget::createConnections() {
	connect(m_pUpdateTimer, &QTimer::timeout, this,
			&AudioChannelFeatureWidget::audioChannelValueChanged);
}

bool AudioChannelFeatureWidget::getLinkStatus() {
	if (this->m_pRetSetAudioControlDetailValue)
		return this->m_pRetSetAudioControlDetailValue->getSteroLink();
	return false;
}

QString AudioChannelFeatureWidget::getChannelName() {
	return this->m_pAudioControlDetail->getChannelName().c_str();
}

int AudioChannelFeatureWidget::getChannelId() const { return m_iChannelId; }

void AudioChannelFeatureWidget::setChannelId(int value) {
	m_iChannelId = value;
}

void AudioChannelFeatureWidget::audioChannelValueChanged() {
	try {
		m_pRetSetAudioControlDetailValue->execute();
	} catch (CommunicationException e) {
		std::cerr << e.getErrorMessage() << std::endl;
	}
	m_pRetSetAudioControlDetailValue->reset();
}

void AudioChannelFeatureWidget::changeLinkStatus(
	__attribute__((unused)) unsigned int detailId, bool enabled) {
	this->ui->m_pChbStereoLink->setChecked(enabled);
	this->m_pRetSetAudioControlDetailValue->setSteroLink(enabled);
	this->m_pUpdateTimer->start(10);
}

void AudioChannelFeatureWidget::changePhantomPowerStatus(bool enabled) {
	this->ui->m_pChbPhantomPower->setChecked(enabled);
}

void AudioChannelFeatureWidget::changeHighImpedanceStatus(bool enabled) {
	this->ui->m_pChbHighImpedance->setChecked(enabled);
}

void AudioChannelFeatureWidget::changeMuteStatus(bool enabled) {
	this->ui->m_pChbMute->setChecked(enabled);
}

void AudioChannelFeatureWidget::changeVolume(int volume) {
	this->ui->m_pSlideVolume->setValue(volume);
}

void AudioChannelFeatureWidget::changeTrim(int trim) {
	this->ui->m_pDialTrim->setValue(trim);
}

void AudioChannelFeatureWidget::setVolume(int volume) {
	this->ui->m_pPBLeft->setValue(volume);
}

void AudioChannelFeatureWidget::changeMeterVolume(int channel, int value) {
	if (channel == m_iChannelId) {
		ui->m_pPBLeft->setValue(value);
	}
	if (m_bIsMaster && channel == m_iChannelId + 1)
		ui->m_pPBRight->setValue(value);
}
