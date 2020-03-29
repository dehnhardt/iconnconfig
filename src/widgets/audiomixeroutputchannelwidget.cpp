#include "audiomixeroutputchannelwidget.h"
#include "../sysex/getmixeroutputcontrolvalue.h"
#include "../sysex/getmixeroutputparm.h"
#include "../sysex/protocolexception.h"
#include "../sysex/retsetaudiochannelname.h"
#include "portdisplayhelper.h"
#include "ui_audiochannelfeaturewidget.h"

#include <QMenu>
#include <QVBoxLayout>
#include <sstream>

AudioMixerOutputChannelWidget::AudioMixerOutputChannelWidget(
	Device *device, unsigned int portId, unsigned int channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: AudioMixerChannelWidget(device, portId, channelNumber, channelDirection,
							  parent) {

	ui->setupUi(this);
	initControls();
	std::unique_ptr<GetMixerOutputParm> getMixerOutputParm =
		std::make_unique<GetMixerOutputParm>(device);
	getMixerOutputParm->setPortId(m_iPortId);
	getMixerOutputParm->setMixerOutputNumber(m_iMixerChannelId);
	m_pMixerOutputParm = std::dynamic_pointer_cast<RetSetMixerOutputParm>(
		getMixerOutputParm->querySmart());
	ui->m_pLblChannelName->setText(m_pMixerOutputParm->getMixerName().c_str());
	QVBoxLayout *l =
		static_cast<QVBoxLayout *>(ui->m_pFrmUpperMixerPanel->layout());
	if (l) {
		m_pBtnSelectConnection = new QToolButton();
		m_pBtnSelectConnection->setCheckable(false);
		l->addWidget(new QLabel(tr("In")));
		l->addWidget(m_pBtnSelectConnection);
		createOutputMenu();
		m_pBtnSelectConnection->setMenu(m_pConnectionMenu);
		m_pBtnSelectConnection->setPopupMode(
			QToolButton::ToolButtonPopupMode::MenuButtonPopup);
		m_pBtnSelectConnection->setToolButtonStyle(
			Qt::ToolButtonTextBesideIcon);
	}

	ui->m_pSlideVolume->setDebug(false);
}

AudioMixerOutputChannelWidget::AudioMixerOutputChannelWidget(QWidget *parent)
	: AudioMixerChannelWidget(parent) {
	ui->setupUi(this);
}

AudioMixerOutputChannelWidget::~AudioMixerOutputChannelWidget() {}

void AudioMixerOutputChannelWidget::setMixerOutputControl(
	std::shared_ptr<RetMixerOutputControl> retMixerOutputControl) {

	this->m_pMixerOutputControl = retMixerOutputControl;

	ui->m_pSlidePan->setVisible(m_pMixerOutputControl->hasPanControl());
	ui->m_pSlideVolume->setVisible(m_pMixerOutputControl->hasVolumeControl());
	ui->m_pTbMute->setVisible(m_pMixerOutputControl->hasMuteControl());
	ui->m_pDial->setVisible(m_pMixerOutputControl->hasSoloControl());
	ui->m_pTbPfl->setVisible(m_pMixerOutputControl->hasSoloPFLControl());
	ui->m_pTbInvert->setVisible(m_pMixerOutputControl->hasInvertControl());
	ui->m_pChbStereoLink->setVisible(
		m_pMixerOutputControl->hasStereoLinkControl());

	ui->m_pSlidePan->setEnabled(m_pMixerOutputControl->getPanControlEditable());
	ui->m_pSlideVolume->setVisible(m_pMixerOutputControl->hasVolumeControl());
	ui->m_pTbMute->setEnabled(m_pMixerOutputControl->getMuteControlEditable());
	ui->m_pDial->setEnabled(m_pMixerOutputControl->getSoloControlEditable());
	ui->m_pTbPfl->setEnabled(
		m_pMixerOutputControl->getSoloPFLControlEditable());
	ui->m_pTbInvert->setEnabled(
		m_pMixerOutputControl->getInvertControlEditable());
	// At least model IConnectAudio4+ only allows stereo-outputs in mixer
	// channels
	if (m_pDevice->getProductId()->getLongValue() == 7)
		ui->m_pChbStereoLink->setEnabled(false);
	else
		ui->m_pChbStereoLink->setEnabled(
			m_pMixerOutputControl->getStereoLinkControlEditable());

	if (m_pMixerOutputControl->hasPanControl()) {
		ui->m_pSlidePan->setMinimum(m_pMixerOutputControl->getMaxPanValue() *
									-1);
		ui->m_pSlidePan->setMaximum(m_pMixerOutputControl->getMaxPanValue());
		if (m_pMixerOutputControl->getPanControlEditable()) {
			connect(this->ui->m_pSlidePan, &QDial::valueChanged,
					[=](int value) {
						this->m_pRetSetMixerOutputControlValue->setPan(value);
						this->m_pUpdateTimer->start();
						emit panChanged(value);
					});
		}
	}
	if (m_pMixerOutputControl->hasVolumeControl()) {
		ui->m_pSlideVolume->setFontSize(7);
		this->m_pChannelCalc = std::make_shared<IConnCalc>(
			0, 256, m_pMixerOutputControl->getMinimumVolumeValue(),
			m_pMixerOutputControl->getMaximumVolumeValue(), 256);
		ui->m_pSlideVolume->setScaleCalc(this->m_pChannelCalc);
		if (m_pMixerOutputControl->getVolumeControlEditable()) {
			ui->m_pSlidePan->setEnabled(m_pMixerOutputControl->hasPanControl());
			connect(this->ui->m_pSlideVolume, &PKSlider::valueChanged,
					[=](int value) {
						this->m_pRetSetMixerOutputControlValue->setVolume(
							value);
						this->m_pUpdateTimer->start();
						emit volumeChanged(value);
					});
		}
	}
	if (m_pMixerOutputControl->hasMuteControl()) {
		if (m_pMixerOutputControl->getMuteControlEditable()) {
			connect(this->ui->m_pTbMute, &QToolButton::toggled,
					[=](bool state) {
						this->m_pRetSetMixerOutputControlValue->setMute(state);
						this->m_pUpdateTimer->start();
						emit muteStatusChanged(state);
					});
		}
	}
	if (m_pMixerOutputControl->hasSoloControl()) {
		this->ui->m_plblTrimPan->setText(tr("Solo Volume"));
		this->ui->m_plblTrimPan->setVisible(true);
		float min = this->m_pChannelCalc->encode(
			m_pMixerOutputControl->getMinimumVolumeValue());
		float max = this->m_pChannelCalc->encode(
			m_pMixerOutputControl->getMaximumVolumeValue());
		std::cout << "Solo min: " << min << " (" << std::dec
				  << m_pMixerOutputControl->getMinimumVolumeValue()
				  << "), Solo max: " << max << ",(" << std::dec
				  << m_pMixerOutputControl->getMaximumVolumeValue() << ")"
				  << std::endl;
		ui->m_pDial->setMinimum(min);
		ui->m_pDial->setMaximum(max);
		if (m_pMixerOutputControl->getSoloControlEditable()) {
			connect(ui->m_pDial, &QDial::valueChanged, [=](int value) {
				float decoded = this->m_pChannelCalc->decode(value);
				std::cout << "Set Solo-Value " << decoded << "(" << std::dec
						  << value << ")" << std::endl;
				this->m_pRetSetMixerOutputControlValue->setSolo(decoded);
				this->m_pUpdateTimer->start();
				emit soloStatusChanged(value);
			});
		}
	}
	if (m_pMixerOutputControl->hasSoloPFLControl()) {
		if (m_pMixerOutputControl->getSoloPFLControlEditable()) {
			connect(ui->m_pTbPfl, &QToolButton::clicked, [=](bool state) {
				this->m_pRetSetMixerOutputControlValue->setSoloPFL(state);
				this->m_pUpdateTimer->start();
				emit soloPFLStatusChanged(state);
			});
		}
	}
	if (m_pMixerOutputControl->hasInvertControl()) {
		if (m_pMixerOutputControl->getInvertControlEditable()) {
			connect(ui->m_pTbInvert, &QToolButton::clicked, [=](bool state) {
				this->m_pRetSetMixerOutputControlValue->setInvert(state);
				this->m_pUpdateTimer->start();
				emit invertStatusChanged(state);
			});
		}
	}
	if (m_pMixerOutputControl->hasStereoLinkControl()) {
		if (m_pMixerOutputControl->getStereoLinkControlEditable()) {
			connect(ui->m_pChbStereoLink, &QToolButton::clicked,
					[=](bool clicked) {
						this->m_pRetSetMixerOutputControlValue->setSteroLink(
							clicked);
						this->m_pUpdateTimer->start();
						emit linkStatusChanged(m_iMixerChannelId,
											   m_channelDirection, clicked);
					});
		}
	}
	channelInit = true;
}

void AudioMixerOutputChannelWidget::setMaster(bool isMaster,
											  QString channel2Name) {
	this->ui->m_pPBRight->setVisible(isMaster);
	m_bIsMaster = isMaster;
	/*if (isMaster) {
		QString name = this->getChannelName();
		name.append(" / ");
		name.append(channel2Name);
		ui->m_pLblChannelName->setText(name);
	} else {
		ui->m_pLblChannelName->setText(this->getChannelName());
	}*/
}

void AudioMixerOutputChannelWidget::createOutputMenu() {
	QFont menuFont = ui->m_pFrmUpperMixerPanel->font();
	m_pConnectionMenu = new QMenu();
	m_pConnectionMenu->setFont(menuFont);
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	QAction *qANone = m_pConnectionMenu->addAction(tr(" - none - "));
	QAction *qAMultiple = m_pConnectionMenu->addAction(tr(" - multiple - "));
	connect(qANone, &QAction::triggered, [=]() {
		std::cout << "Audio port selected" << std::endl;
		changeOutput(0, "", 0, "", AudioPortType::APT_NONE);
	});
	connect(qAMultiple, &QAction::triggered, [=]() {
		std::cout << "Audio port selected" << std::endl;
		changeOutput(0, "", 0, "", AudioPortType::APT_NONE);
	});
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		AudioPortType audioPortType = static_cast<AudioPortType>(section);

		std::string portTypeName =
			PortDisplayHelper::getAudioPortTypeName(audioPortType);
		QIcon portIcon =
			PortDisplayHelper::getAudioPortIcon(AudioPortType(section));
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			unsigned int portId = app->getPortId();
			if (portId != m_iPortId)
				continue;
			AudioChannelNames::iterator channelIt;
			AudioChannelNames channelNames = m_pDevice->getAudioChannelNames(
				portId, ChannelDirection::CD_OUTPUT);
			for (channelIt = channelNames.begin();
				 channelIt != channelNames.end(); ++channelIt) {
				unsigned int channelId = channelIt->first;
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelIt->second;
				QAction *a = m_pConnectionMenu->addAction(
					portIcon, QString::number(channelId) + ": " +
								  channelName->getChannelName().c_str());
				a->setCheckable(true);
				a->setProperty("channelId", channelId);
				connect(a, &QAction::triggered, [=]() {
					std::cout << "Audio port selected" << std::endl;
					changeOutput(portId, app->getPortName(), channelId,
								 channelName->getChannelName(),
								 AudioPortType(section));
				});
			}
		}
	}
}

void AudioMixerOutputChannelWidget::setOutput(
	std::vector<unsigned int> audioSinkChannelNumbers) {
	if (audioSinkChannelNumbers.size() == 0) {
		setOutputName(0, "none", 0, "", AudioPortType::APT_NONE);
		return;
	} else if (audioSinkChannelNumbers.size() > 1) {
		setOutputName(0, "multiple", 0, "", AudioPortType::APT_NONE);
	}
	for (unsigned int channel : audioSinkChannelNumbers) {
		for (QAction *action : m_pConnectionMenu->actions()) {
			if (action->property("channelId") == channel) {
				action->setChecked(true);
				if (audioSinkChannelNumbers.size() == 1) {
					if (channel == 0) {
						setOutputName(0, "none", 0, "",
									  AudioPortType::APT_NONE);
					} else {
						AudioChannelNames channelNames =
							m_pDevice->getAudioChannelNames(
								m_iPortId, ChannelDirection::CD_OUTPUT);
						std::shared_ptr<RetSetAudioChannelName> channelName =
							channelNames[channel];
						setOutputName(m_iPortId, "none", channel,
									  channelName->getChannelName(),
									  AudioPortType::APT_NONE);
					}
					break;
				}
			}
		}
	}
}

void AudioMixerOutputChannelWidget::setOutputName(
	unsigned int audioSinkPortId, std::string audioSinkPortName,
	unsigned int audioSinkChannelNumber, std::string audioSinkChannelName,
	AudioPortType audioPortType) {
	if (audioSinkPortId == 0 || audioSinkChannelNumber == 0) {
		if (audioSinkPortName == "none") {
			m_pBtnSelectConnection->setToolTip(tr("none"));
			m_pBtnSelectConnection->setText(tr("none"));
			m_pBtnSelectConnection->setIcon(QIcon());
		} else if (audioSinkPortName == "multiple") {
			m_pBtnSelectConnection->setToolTip(tr("multiple"));
			m_pBtnSelectConnection->setText(tr("multiple"));
			m_pBtnSelectConnection->setIcon(QIcon());
		}
		return;
	}

	std::stringstream name;
	name << tr("Port ").toStdString() << std::dec << audioSinkPortId << ": "
		 << audioSinkPortName << " - " << tr("Channel ").toStdString()
		 << audioSinkChannelNumber << ": " << audioSinkChannelName;
	m_pBtnSelectConnection->setToolTip(name.str().c_str());
	m_pBtnSelectConnection->setText(QString::number(audioSinkPortId) + " - " +
									QString::number(audioSinkChannelNumber));
	m_pBtnSelectConnection->setIcon(
		PortDisplayHelper::getAudioPortIcon(audioPortType));
}

void AudioMixerOutputChannelWidget::queryOutputValues() {
	std::unique_ptr<GetMixerOutputControlValue> getMixerOutputControlValue =
		std::make_unique<GetMixerOutputControlValue>(m_pDevice);
	getMixerOutputControlValue->setPortId(m_iPortId);
	getMixerOutputControlValue->setMixerOutputNumber(m_iMixerChannelId);

	try {
		this->m_pRetSetMixerOutputControlValue =
			std::dynamic_pointer_cast<RetSetMixerOutputControlValue>(
				getMixerOutputControlValue->querySmart());
		this->m_pRetSetMixerOutputControlValue->setDebug(true);
	} catch (ProtocolException protocolException) {
		std::cerr << protocolException.getErrorMessage() << std::endl;
	} catch (std::out_of_range oor) {
		std::cerr << oor.what() << std::endl;
	}
}

void AudioMixerOutputChannelWidget::setOutputValues(
	std::shared_ptr<RetSetMixerOutputControlValue>
		retSetMixerOutputControlValue) {
	/*QString name =
		"CN: " + QString::number(m_iMixerChannelId) + " IN " +
		QString::number(retSetMixerOutputControlValue->getMixerInputNumber()) +
		" ON: " +
		QString::number(retSetMixerOutputControlValue->getMixerOutputNumber());*/
	if (m_pMixerOutputControl->hasPanControl() &&
		retSetMixerOutputControlValue->hasPanControl()) {
		ui->m_pSlidePan->setValue(retSetMixerOutputControlValue->getPan());
		retSetMixerOutputControlValue->setHasPanControl(false);
	}
	if (m_pMixerOutputControl->hasVolumeControl() &&
		retSetMixerOutputControlValue->hasVolumeControl()) {
		ui->m_pSlideVolume->setValue(
			retSetMixerOutputControlValue->getVolume());
		retSetMixerOutputControlValue->setHasVolumeControl(false);
	}
	if (m_pMixerOutputControl->hasMuteControl() &&
		retSetMixerOutputControlValue->hasMuteControl()) {
		ui->m_pTbMute->setChecked(retSetMixerOutputControlValue->getMute());
		retSetMixerOutputControlValue->setHasMuteControl(false);
	}
	if (m_pMixerOutputControl->hasSoloControl()) {
		std::cout << "get Solo Value: (" << m_iMixerChannelId << ") "
				  << retSetMixerOutputControlValue->getSolo() << ", encoded: "
				  << this->m_pChannelCalc->encode(
						 retSetMixerOutputControlValue->getSolo())
				  << ", decoded: "
				  << this->m_pChannelCalc->decode(
						 retSetMixerOutputControlValue->getSolo())
				  << std::endl;
		ui->m_pDial->setValue(this->m_pChannelCalc->encode(
			retSetMixerOutputControlValue->getSolo()));
		retSetMixerOutputControlValue->setHasSoloControl(false);
	}
	if (m_pMixerOutputControl->hasSoloPFLControl()) {
		ui->m_pTbPfl->setChecked(retSetMixerOutputControlValue->getSoloPFL());
		retSetMixerOutputControlValue->setHasSoloPFLControl(false);
	}
	if (m_pMixerOutputControl->hasInvertControl()) {
		ui->m_pTbInvert->setChecked(retSetMixerOutputControlValue->getInvert());
		retSetMixerOutputControlValue->setHasInvertControl(false);
	}
	if (m_pMixerOutputControl->hasStereoLinkControl()) {
		ui->m_pChbStereoLink->setChecked(
			retSetMixerOutputControlValue->getSteroLink());
		retSetMixerOutputControlValue->setHasStereoLinkControl(false);
		if (retSetMixerOutputControlValue->getSteroLink())
			emit linkStatusChanged(m_iMixerChannelId, m_channelDirection, true);
	}
}

void AudioMixerOutputChannelWidget::audioChannelValueChanged() {
	std::cout << "Value Changed " << std::endl;
	if (this->m_pRetSetMixerOutputControlValue->execute()) {
		refreshStatus();
	}
}

void AudioMixerOutputChannelWidget::changeLinkStatus(bool enabled) {
	this->ui->m_pChbStereoLink->setChecked(enabled);
	this->m_pRetSetMixerOutputControlValue->setSteroLink(enabled);
	this->m_pUpdateTimer->start();
}

void AudioMixerOutputChannelWidget::changeOutput(
	unsigned int audioSinkPortId, std::string audioSinkPortName,
	unsigned int audioSinkChannelNumber, std::string audioSinkChannelName,
	AudioPortType audioPortType) {
	setOutputName(audioSinkPortId, audioSinkPortName, audioSinkChannelNumber,
				  audioSinkChannelName, audioPortType);
}

void AudioMixerOutputChannelWidget::refreshOutput() {
	std::unique_ptr<GetMixerOutputParm> getMixerOutputParm =
		std::make_unique<GetMixerOutputParm>(m_pDevice);
	getMixerOutputParm->setPortId(m_iPortId);
	getMixerOutputParm->setMixerOutputNumber(m_iMixerChannelId);
	m_pMixerOutputParm = std::dynamic_pointer_cast<RetSetMixerOutputParm>(
		getMixerOutputParm->querySmart());
	setOutput(m_pMixerOutputParm->getAudioSourceChannelNumbers());
}

void AudioMixerOutputChannelWidget::refreshStatus() {
	refreshOutput();
	changeMeterVolume(m_iMixerChannelId, -10);
	if (m_pMixerOutputControl->hasControls() && channelInit) {
		queryOutputValues();
		if (this->m_pRetSetMixerOutputControlValue)
			setOutputValues(this->m_pRetSetMixerOutputControlValue);
	}
}
