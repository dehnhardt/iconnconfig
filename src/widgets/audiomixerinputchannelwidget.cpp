#include "audiomixerinputchannelwidget.h"
#include "../sysex/getmixerinputcontrolvalue.h"
#include "../sysex/getmixerinputparm.h"
#include "../sysex/getmixeroutputparm.h"
#include "../sysex/protocolexception.h"
#include "../sysex/retsetaudiochannelname.h"
#include "portdisplayhelper.h"
#include "ui_audiochannelfeaturewidget.h"

#include <QMenu>
#include <QVBoxLayout>
#include <sstream>

AudioMixerInputChannelWidget::AudioMixerInputChannelWidget(
	Device *device, unsigned int portId, unsigned int channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: AudioMixerChannelWidget(device, portId, channelNumber, channelDirection,
							  parent) {
	ui->setupUi(this);
	initControls();
	this->ui->m_pLblChannelName->setText(getChannelName());
	QVBoxLayout *l =
		static_cast<QVBoxLayout *>(ui->m_pFrmUpperMixerPanel->layout());
	if (l) {
		m_pBtnSelectConnection = new QToolButton();
		m_pBtnSelectConnection->setCheckable(false);
		l->addWidget(new QLabel(tr("In")));
		l->addWidget(m_pBtnSelectConnection);
		createInputMenu();
		m_pBtnSelectConnection->setMenu(m_pConnectionMenu);
		m_pBtnSelectConnection->setPopupMode(
			QToolButton::ToolButtonPopupMode::MenuButtonPopup);
		m_pBtnSelectConnection->setToolButtonStyle(
			Qt::ToolButtonTextBesideIcon);
		// refreshInput();
	}

	ui->m_pSlideVolume->setDebug(false);
}

AudioMixerInputChannelWidget::AudioMixerInputChannelWidget(QWidget *parent)
	: AudioMixerChannelWidget(parent) {
	ui->setupUi(this);
}

AudioMixerInputChannelWidget::~AudioMixerInputChannelWidget() {}

void AudioMixerInputChannelWidget::setMixerInputControl(
	std::shared_ptr<RetMixerInputControl> retMixerInputControl) {

	this->m_pMixerInputControl = retMixerInputControl;

	ui->m_pDialTrim->setVisible(m_pMixerInputControl->hasPanControl());
	ui->m_pSlideVolume->setVisible(m_pMixerInputControl->hasVolumeControl());
	ui->m_pTbMute->setVisible(m_pMixerInputControl->hasMuteControl());
	ui->m_pTbSolo->setVisible(m_pMixerInputControl->hasSoloControl());
	ui->m_pTbPfl->setVisible(m_pMixerInputControl->hasSoloPFLControl());
	ui->m_pTbInvert->setVisible(m_pMixerInputControl->hasInvertControl());
	ui->m_pChbStereoLink->setVisible(
		m_pMixerInputControl->hasStereoLinkControl());

	ui->m_pDialTrim->setEnabled(m_pMixerInputControl->getPanControlEditable());
	ui->m_pSlideVolume->setVisible(m_pMixerInputControl->hasVolumeControl());
	ui->m_pTbMute->setEnabled(m_pMixerInputControl->getMuteControlEditable());
	ui->m_pTbSolo->setEnabled(m_pMixerInputControl->getSoloControlEditable());
	ui->m_pTbPfl->setEnabled(m_pMixerInputControl->getSoloPFLControlEditable());
	ui->m_pTbInvert->setEnabled(
		m_pMixerInputControl->getInvertControlEditable());
	ui->m_pChbStereoLink->setEnabled(
		m_pMixerInputControl->getStereoLinkControlEditable());

	if (m_pMixerInputControl->hasPanControl()) {
		ui->m_pDialTrim->setMinimum(-127);
		ui->m_pDialTrim->setMaximum(127);
		if (m_pMixerInputControl->getPanControlEditable()) {
			connect(this->ui->m_pDialTrim, &QDial::valueChanged,
					[=](int value) {
						this->m_pRetSetMixerInputControlValue->setPan(value);
						this->m_pUpdateTimer->start();
						std::cout << "Pan: " << std::dec << value << std::endl;
						emit panChanged(value);
					});
		}
	}
	if (m_pMixerInputControl->hasVolumeControl()) {
		ui->m_pSlideVolume->setFontSize(7);
		this->m_pChannelCalc = std::make_shared<IConnCalc>(
			0, 256, m_pMixerInputControl->getMinimumVolumeValue(),
			m_pMixerInputControl->getMaximumVolumeValue(), 256);
		ui->m_pSlideVolume->setScaleCalc(this->m_pChannelCalc);
		if (m_pMixerInputControl->getVolumeControlEditable()) {
			ui->m_pDialTrim->setEnabled(m_pMixerInputControl->hasPanControl());
			connect(this->ui->m_pSlideVolume, &PKSlider::valueChanged,
					[=](int value) {
						this->m_pRetSetMixerInputControlValue->setVolume(value);
						this->m_pUpdateTimer->start();
						emit volumeChanged(value);
					});
		}
	}
	if (m_pMixerInputControl->hasMuteControl()) {
		if (m_pMixerInputControl->getMuteControlEditable()) {
			connect(this->ui->m_pTbMute, &QToolButton::toggled,
					[=](bool state) {
						this->m_pRetSetMixerInputControlValue->setMute(state);
						this->m_pUpdateTimer->start();
						emit muteStatusChanged(state);
					});
		}
	}
	if (m_pMixerInputControl->hasSoloControl()) {
		if (m_pMixerInputControl->getSoloControlEditable()) {
			connect(ui->m_pTbSolo, &QToolButton::clicked, [=](bool state) {
				this->m_pRetSetMixerInputControlValue->setSolo(state);
				this->m_pUpdateTimer->start();
				emit soloStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasSoloPFLControl()) {
		if (m_pMixerInputControl->getSoloPFLControlEditable()) {
			connect(ui->m_pTbPfl, &QToolButton::clicked, [=](bool state) {
				this->m_pRetSetMixerInputControlValue->setSoloPFL(state);
				this->m_pUpdateTimer->start();
				emit soloPFLStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasInvertControl()) {
		if (m_pMixerInputControl->getInvertControlEditable()) {
			connect(ui->m_pTbInvert, &QToolButton::clicked, [=](bool state) {
				this->m_pRetSetMixerInputControlValue->setInvert(state);
				this->m_pUpdateTimer->start();
				emit invertStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasStereoLinkControl()) {
		if (m_pMixerInputControl->getStereoLinkControlEditable()) {
			connect(ui->m_pChbStereoLink, &QToolButton::clicked,
					[=](bool clicked) {
						this->m_pRetSetMixerInputControlValue->setSteroLink(
							clicked);
						this->m_pUpdateTimer->start();
						emit linkStatusChanged(m_iMixerChannelId,
											   m_channelDirection, clicked);
					});
		}
	}
	channelInit = true;
}

void AudioMixerInputChannelWidget::createInputMenu() {
	QFont menuFont = ui->m_pFrmUpperMixerPanel->font();
	m_pConnectionMenu = new QMenu();
	m_pConnectionMenu->setFont(menuFont);
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	QAction *qANone = m_pConnectionMenu->addAction(tr(" - none - "));
	connect(qANone, &QAction::triggered, [=]() {
		std::cout << "Audio port selected" << std::endl;
		changeInput(0, "", 0, "", AudioPortType::APT_NONE);
	});
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		AudioPortType audioPortType = static_cast<AudioPortType>(section);
		std::string portTypeName =
			PortDisplayHelper::getAudioPortTypeName(audioPortType);
		QIcon portIcon =
			PortDisplayHelper::getAudioPortIcon(AudioPortType(section));
		QMenu *audioPortTypeMenu =
			m_pConnectionMenu->addMenu(portIcon, tr(portTypeName.c_str()));
		audioPortTypeMenu->setFont(menuFont);
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			unsigned int portId = app->getPortId();
			QMenu *portMenu = audioPortTypeMenu->addMenu(
				portIcon,
				QString::number(portId) + ": " + app->getPortName().c_str());
			portMenu->setFont(menuFont);
			AudioChannelNames::iterator channelIt;
			AudioChannelNames channelNames = m_pDevice->getAudioChannelNames(
				portId, ChannelDirection::CD_INPUT);
			for (channelIt = channelNames.begin();
				 channelIt != channelNames.end(); ++channelIt) {
				unsigned int channelId = channelIt->first;
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelIt->second;
				QAction *a = portMenu->addAction(
					portIcon, QString::number(channelId) + ": " +
								  channelName->getChannelName().c_str());
				connect(a, &QAction::triggered, [=]() {
					std::cout << "Audio port selected" << std::endl;
					changeInput(portId, app->getPortName(), channelId,
								channelName->getChannelName(),
								AudioPortType(section));
				});
			}
		}
	}
}

void AudioMixerInputChannelWidget::setInput(
	unsigned int audioSourcePortId, unsigned int audioSourceChannelNumber) {
	if (audioSourcePortId == 0 || audioSourceChannelNumber == 0) {
		setInputName(0, "", 0, "", AudioPortType::APT_NONE);
	}
	m_iSourcePortId = audioSourcePortId;
	m_iSourceChannelId = audioSourceChannelNumber;
	std::shared_ptr<AudioPortStructure> aps =
		m_pDevice->getAudioPortStructure();
	AudioPortStructure::iterator itPortType;
	for (itPortType = aps->begin(); itPortType != aps->end(); ++itPortType) {
		int section = itPortType->first;
		std::vector<std::shared_ptr<RetSetAudioPortParm>>::iterator itPort;
		std::vector<std::shared_ptr<RetSetAudioPortParm>> *ports =
			itPortType->second;
		for (itPort = ports->begin(); itPort != ports->end(); itPort++) {
			std::shared_ptr<RetSetAudioPortParm> app = *itPort;
			if (audioSourcePortId == app->getPortId()) {
				AudioChannelNames channelNames =
					m_pDevice->getAudioChannelNames(audioSourcePortId,
													ChannelDirection::CD_INPUT);
				std::shared_ptr<RetSetAudioChannelName> channelName =
					channelNames[audioSourceChannelNumber];
				setInputName(audioSourcePortId, app->getPortName(),
							 audioSourceChannelNumber,
							 channelName->getChannelName(),
							 static_cast<AudioPortType>(section));
				return;
			}
		}
	}
}

void AudioMixerInputChannelWidget::setInputName(
	unsigned int audioSourcePortId, std::string audioSourcePortName,
	unsigned int audioSourceChannelNumber, std::string audioSourceChannelName,
	AudioPortType audioPortType) {
	if (audioSourcePortId == 0 || audioSourceChannelNumber == 0) {
		m_pBtnSelectConnection->setToolTip(tr("none"));
		m_pBtnSelectConnection->setText(tr("none"));
		m_pBtnSelectConnection->setIcon(QIcon());
		return;
	}
	std::stringstream name;
	name << tr("Port ").toStdString() << std::dec << audioSourcePortId << ": "
		 << audioSourcePortName << " - " << tr("Channel ").toStdString()
		 << audioSourceChannelNumber << ": " << audioSourceChannelName;
	m_pBtnSelectConnection->setToolTip(name.str().c_str());
	m_pBtnSelectConnection->setText(QString::number(audioSourcePortId) + " - " +
									QString::number(audioSourceChannelNumber));
	m_pBtnSelectConnection->setIcon(
		PortDisplayHelper::getAudioPortIcon(audioPortType));
}

void AudioMixerInputChannelWidget::changeInput(
	unsigned int audioSourcePortId, std::string audioSourcePortName,
	unsigned int audioSourceChannelNumber, std::string audioSourceChannelName,
	AudioPortType audioPortType) {
	setInputName(audioSourcePortId, audioSourcePortName,
				 audioSourceChannelNumber, audioSourceChannelName,
				 audioPortType);
}

void AudioMixerInputChannelWidget::queryInputValues() {
	std::unique_ptr<GetMixerInputControlValue> getMixerInputControlValue =
		std::make_unique<GetMixerInputControlValue>(m_pDevice);
	getMixerInputControlValue->setPortId(m_iPortId);
	getMixerInputControlValue->setMixerOutputNumber(
		m_pMixerInputParm->getAudioSourcePortId());
	getMixerInputControlValue->setMixerInputNumber(m_iMixerChannelId);

	try {
		m_pRetSetMixerInputControlValue =
			std::dynamic_pointer_cast<RetSetMixerInputControlValue>(
				getMixerInputControlValue->querySmart());
	} catch (ProtocolException protocolException) {
		std::cerr << protocolException.getErrorMessage() << std::endl;
	} catch (std::out_of_range oor) {
		std::cerr << oor.what() << std::endl;
	}
}

void AudioMixerInputChannelWidget::setInputValues(
	std::shared_ptr<RetSetMixerInputControlValue>
		retSetMixerInputControlValue) {
	/*QString name =
		"CN: " + QString::number(m_iMixerChannelId) + " IN " +
		QString::number(retSetMixerInputControlValue->getMixerInputNumber()) +
		" ON: " +
		QString::number(retSetMixerInputControlValue->getMixerOutputNumber());*/
	if (m_pMixerInputControl->hasPanControl() &&
		retSetMixerInputControlValue->hasPanControl()) {
		std::cout << "Pan init: " << retSetMixerInputControlValue->getPan()
				  << std::endl;
		ui->m_pDialTrim->setValue(retSetMixerInputControlValue->getPan());
		retSetMixerInputControlValue->setHasPanControl(false);
	}
	if (m_pMixerInputControl->hasVolumeControl() &&
		retSetMixerInputControlValue->hasVolumeControl()) {
		ui->m_pSlideVolume->setValue(retSetMixerInputControlValue->getVolume());
		retSetMixerInputControlValue->setHasVolumeControl(false);
	}
	if (m_pMixerInputControl->hasMuteControl() &&
		retSetMixerInputControlValue->hasMuteControl()) {
		ui->m_pTbMute->setChecked(retSetMixerInputControlValue->getMute());
		retSetMixerInputControlValue->setHasMuteControl(false);
	}
	if (m_pMixerInputControl->hasSoloControl()) {
		ui->m_pTbSolo->setChecked(retSetMixerInputControlValue->getSolo());
		retSetMixerInputControlValue->setHasSoloControl(false);
	}
	if (m_pMixerInputControl->hasSoloPFLControl()) {
		ui->m_pTbPfl->setChecked(retSetMixerInputControlValue->getSoloPFL());
		retSetMixerInputControlValue->setHasSoloPFLControl(false);
	}
	if (m_pMixerInputControl->hasInvertControl()) {
		ui->m_pTbInvert->setChecked(retSetMixerInputControlValue->getInvert());
		retSetMixerInputControlValue->setHasInvertControl(false);
	}
	if (m_pMixerInputControl->hasStereoLinkControl()) {
		ui->m_pChbStereoLink->setChecked(
			retSetMixerInputControlValue->getSteroLink());
		retSetMixerInputControlValue->setHasStereoLinkControl(false);
		if (retSetMixerInputControlValue->getSteroLink())
			emit linkStatusChanged(m_iMixerChannelId, m_channelDirection, true);
	}
}

void AudioMixerInputChannelWidget::audioChannelValueChanged() {
	if (this->m_pRetSetMixerInputControlValue->execute()) {
		refreshStatus();
	}
}

void AudioMixerInputChannelWidget::changeLinkStatus(bool enabled) {
	this->ui->m_pChbStereoLink->setChecked(enabled);
	this->m_pRetSetMixerInputControlValue->setSteroLink(enabled);
	this->m_pUpdateTimer->start();
}

void AudioMixerInputChannelWidget::setMaster(bool isMaster,
											 QString channel2Name) {
	this->ui->m_pPBRight->setVisible(isMaster);
	m_bIsMaster = isMaster;
	if (isMaster) {
		QString name = this->getChannelName();
		name.append(" / ");
		name.append(channel2Name);
		ui->m_pLblChannelName->setText(name);
	} else {
		ui->m_pLblChannelName->setText(this->getChannelName());
	}
}

void AudioMixerInputChannelWidget::refreshInput() {
	std::unique_ptr<GetMixerInputParm> getMixerInputParm =
		std::make_unique<GetMixerInputParm>(m_pDevice);
	getMixerInputParm->setPortId(m_iPortId);
	getMixerInputParm->setMixerInputNumber(m_iMixerChannelId);
	m_pMixerInputParm = std::dynamic_pointer_cast<RetSetMixerInputParm>(
		getMixerInputParm->querySmart());
	setInput(m_pMixerInputParm->getAudioSourcePortId(),
			 m_pMixerInputParm->getAudioSourceChannelNumber());
}

void AudioMixerInputChannelWidget::refreshStatus() {
	refreshInput();
	changeMeterVolume(m_iMixerChannelId, -10);
	if (m_pMixerInputControl->hasControls() && channelInit &&
		m_iSourceChannelId && m_iSourcePortId) {
		queryInputValues();
		if (this->m_pRetSetMixerInputControlValue)
			setInputValues(this->m_pRetSetMixerInputControlValue);
	}
}
