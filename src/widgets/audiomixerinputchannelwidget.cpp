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
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget), m_pDevice(device),
	  m_iPortId(portId), m_iChannelNumber(channelNumber),
	  m_channelDirection(channelDirection) {
	ui->setupUi(this);
	initControls();
	QVBoxLayout *l =
		static_cast<QVBoxLayout *>(ui->m_pFrmUpperMixerPanel->layout());
	std::unique_ptr<GetMixerInputParm> getMixerInputParm =
		std::make_unique<GetMixerInputParm>(device);
	getMixerInputParm->setPortId(m_iPortId);
	getMixerInputParm->setMixerInputNumber(m_iChannelNumber);
	m_pMixerInputParm = std::dynamic_pointer_cast<RetSetMixerInputParm>(
		getMixerInputParm->querySmart());
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
		setInput(m_pMixerInputParm->getAudioSourcePortId(),
				 m_pMixerInputParm->getAudioSourceChannelNumber());
	}
	ui->m_pSlideVolume->setDebug(false);
}

AudioMixerInputChannelWidget::AudioMixerInputChannelWidget(QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {
	ui->setupUi(this);
}

AudioMixerInputChannelWidget::~AudioMixerInputChannelWidget() {}

void AudioMixerInputChannelWidget::setMixerInputControl(
	std::shared_ptr<RetMixerInputControl> retMixerInputControl) {
	this->m_pMixerInputControl = retMixerInputControl;
	ui->m_pSlideVolume->setFontSize(7);
	this->m_pChannelCalc = std::make_shared<IConnCalc>(
		0, 256, m_pMixerInputControl->getMinimumVolumeValue(),
		m_pMixerInputControl->getMaximumVolumeValue(), 256);
	ui->m_pSlideVolume->setScaleCalc(this->m_pChannelCalc);
	ui->m_pTbMute->setVisible(m_pMixerInputControl->hasMuteControl());
	ui->m_pTbSolo->setVisible(m_pMixerInputControl->hasSoloControl());
	ui->m_pTbPfl->setVisible(m_pMixerInputControl->hasSoloPFLControl());
	ui->m_pTbInvert->setVisible(m_pMixerInputControl->hasInvertControl());

	ui->m_pTbMute->setEnabled(m_pMixerInputControl->getMuteControlEditable());
	ui->m_pTbSolo->setEnabled(m_pMixerInputControl->getSoloControlEditable());
	ui->m_pTbPfl->setEnabled(m_pMixerInputControl->getSoloPFLControlEditable());
	ui->m_pTbInvert->setEnabled(
		m_pMixerInputControl->getInvertControlEditable());

	if (m_pMixerInputControl->hasControls()) {
		queryInputValues();
		if (this->m_pretSetMixerInputControlValue)
			setInputValues(this->m_pretSetMixerInputControlValue);
		m_pUpdateTimer = new QTimer();
		m_pUpdateTimer->setSingleShot(true);
	}
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

void AudioMixerInputChannelWidget::initControls() {
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

void AudioMixerInputChannelWidget::queryInputValues() {
	std::unique_ptr<GetMixerInputControlValue> getMixerInputControlValue =
		std::make_unique<GetMixerInputControlValue>(m_pDevice);
	getMixerInputControlValue->setPortId(m_iPortId);
	getMixerInputControlValue->setMixerOutputNumber(
		m_pMixerInputParm->getAudioSourcePortId());
	getMixerInputControlValue->setMixerInputNumber(m_iChannelNumber);
	// getMixerInputControlValue->setMixerInputNumber(1);

	try {
		m_pretSetMixerInputControlValue =
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
	if (m_pMixerInputControl->hasVolumeControl() &&
		retSetMixerInputControlValue->hasVolumeControl()) {
		ui->m_pSlideVolume->setValue(retSetMixerInputControlValue->getVolume());
		retSetMixerInputControlValue->setHasVolumeControl(false);
		if (m_pMixerInputControl->getVolumeControlEditable()) {
			connect(this->ui->m_pSlideVolume, &PKSlider::valueChanged,
					[=](int value) {
						this->m_pretSetMixerInputControlValue->setVolume(value);
						this->m_pUpdateTimer->start(10);
						emit this->volumeChanged(value);
					});
		}
	}
	if (m_pMixerInputControl->hasMuteControl() &&
		retSetMixerInputControlValue->hasMuteControl()) {
		ui->m_pTbMute->setChecked(retSetMixerInputControlValue->getMute());
		retSetMixerInputControlValue->setHasMuteControl(false);
		if (m_pMixerInputControl->getMuteControlEditable()) {
			connect(this->ui->m_pTbMute, &QToolButton::toggled,
					[=](bool state) {
						this->m_pretSetMixerInputControlValue->setMute(state);
						this->m_pUpdateTimer->start(10);
						emit this->muteStatusChanged(state);
					});
		}
	}
	if (m_pMixerInputControl->hasSoloControl()) {
		ui->m_pTbSolo->setChecked(retSetMixerInputControlValue->getSolo());
		retSetMixerInputControlValue->setHasSoloControl(false);
		if (m_pMixerInputControl->getSoloControlEditable()) {
			connect(ui->m_pTbSolo, &QToolButton::clicked, [=](bool state) {
				this->m_pretSetMixerInputControlValue->setSolo(state);
				this->m_pUpdateTimer->start(10);
				emit this->soloStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasSoloPFLControl()) {
		ui->m_pTbPfl->setChecked(retSetMixerInputControlValue->getSoloPFL());
		retSetMixerInputControlValue->setHasSoloPFLControl(false);
		if (m_pMixerInputControl->getSoloPFLControlEditable()) {
			connect(ui->m_pTbPfl, &QToolButton::clicked, [=](bool state) {
				this->m_pretSetMixerInputControlValue->setSoloPFL(state);
				this->m_pUpdateTimer->start(10);
				emit this->soloPFLStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasInvertControl()) {
		ui->m_pTbInvert->setChecked(retSetMixerInputControlValue->getInvert());
		retSetMixerInputControlValue->setHasInvertControl(false);
		if (m_pMixerInputControl->getInvertControlEditable()) {
			connect(ui->m_pTbInvert, &QToolButton::clicked, [=](bool state) {
				this->m_pretSetMixerInputControlValue->setInvert(state);
				this->m_pUpdateTimer->start(10);
				emit this->invertStatusChanged(state);
			});
		}
	}
	if (m_pMixerInputControl->hasStereoLinkControl()) {
		ui->m_pChbStereoLink->setChecked(
			retSetMixerInputControlValue->getSteroLink());
		retSetMixerInputControlValue->setHasStereoLinkControl(false);
		if (m_pMixerInputControl->getStereoLinkControlEditable()) {
			connect(ui->m_pChbStereoLink, &QToolButton::clicked,
					[=](bool clicked) {
						this->m_pretSetMixerInputControlValue->setSteroLink(
							clicked);
						this->m_pUpdateTimer->start(10);
						emit this->linkStatusChanged(m_iChannelNumber, clicked);
					});
		}
	}
}

void AudioMixerInputChannelWidget::changeMeterVolume(unsigned int channel,
													 int value) {
	if (channel == m_iChannelNumber) {
		ui->m_pPBLeft->setValue(value);
	}
	if (m_bIsMaster && channel == m_iChannelNumber + 1)
		ui->m_pPBRight->setValue(value);
}
