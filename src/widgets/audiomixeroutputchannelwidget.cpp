#include "audiomixeroutputchannelwidget.h"
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

AudioMixerOutputChannelWidget::AudioMixerOutputChannelWidget(
	Device *device, unsigned int portId, unsigned int channelNumber,
	ChannelDirection channelDirection, QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget), m_pDevice(device),
	  m_iPortId(portId), m_iChannelNumber(channelNumber),
	  m_channelDirection(channelDirection) {
	ui->setupUi(this);
	initControls();
	QVBoxLayout *l =
		static_cast<QVBoxLayout *>(ui->m_pFrmUpperMixerPanel->layout());
	std::unique_ptr<GetMixerOutputParm> getMixerOutputParm =
		std::make_unique<GetMixerOutputParm>(device);
	getMixerOutputParm->setPortId(m_iPortId);
	getMixerOutputParm->setMixerOutputNumber(m_iChannelNumber);
	m_pMixerOutputParm = std::dynamic_pointer_cast<RetSetMixerOutputParm>(
		getMixerOutputParm->querySmart());
	ui->m_pLblChannelName->setText(m_pMixerOutputParm->getMixerName().c_str());
	if (l) {
		m_pBtnSelectConnection = new QToolButton();
		m_pBtnSelectConnection->setCheckable(false);
		l->addWidget(new QLabel(tr("Out")));
		l->addWidget(m_pBtnSelectConnection);
		createOutputMenu();
		m_pBtnSelectConnection->setMenu(m_pConnectionMenu);
		m_pBtnSelectConnection->setPopupMode(
			QToolButton::ToolButtonPopupMode::MenuButtonPopup);
		m_pBtnSelectConnection->setToolButtonStyle(
			Qt::ToolButtonTextBesideIcon);
		setOutput(m_pMixerOutputParm->getAudioSourceChannelNumbers());
	}
	ui->m_pSlideVolume->setDebug(false);
}

AudioMixerOutputChannelWidget::AudioMixerOutputChannelWidget(QWidget *parent)
	: QFrame(parent), ui(new Ui::AudioChannelFeatureWidget) {
	ui->setupUi(this);
}

AudioMixerOutputChannelWidget::~AudioMixerOutputChannelWidget() {}

void AudioMixerOutputChannelWidget::setMixerOutputControl(
	std::shared_ptr<RetMixerOutputControl> retMixerOutputControl) {
	this->m_pMixerOutputControl = retMixerOutputControl;
	ui->m_pSlideVolume->setFontSize(7);
	this->m_pChannelCalc = std::make_shared<IConnCalc>(
		0, 256, m_pMixerOutputControl->getMinimumVolumeValue(),
		m_pMixerOutputControl->getMaximumVolumeValue(), 256);
	ui->m_pSlideVolume->setScaleCalc(this->m_pChannelCalc);
	ui->m_pTbMute->setVisible(m_pMixerOutputControl->hasMuteControl());
	ui->m_pTbSolo->setVisible(m_pMixerOutputControl->hasSoloControl());
	ui->m_pTbPfl->setVisible(m_pMixerOutputControl->hasSoloPFLControl());
	ui->m_pTbInvert->setVisible(m_pMixerOutputControl->hasInvertControl());

	ui->m_pTbMute->setEnabled(m_pMixerOutputControl->getMuteControlEditable());
	ui->m_pTbSolo->setEnabled(m_pMixerOutputControl->getSoloControlEditable());
	ui->m_pTbPfl->setEnabled(
		m_pMixerOutputControl->getSoloPFLControlEditable());
	ui->m_pTbInvert->setEnabled(
		m_pMixerOutputControl->getInvertControlEditable());
	if (m_pMixerOutputControl->hasControls()) {
		queryOutputValues();
		/*if (this->m_pretSetMixerOutputControlValue)
			setOutputValues(this->m_pretSetMixerInputControlValue);
		m_pUpdateTimer = new QTimer();
		m_pUpdateTimer->setSingleShot(true);*/
	}
}

void AudioMixerOutputChannelWidget::initControls() {
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
	/*std::unique_ptr<GetMixerOutputControlValue> getMixerOutputControlValue =
		std::make_unique<GetMixerOutputControlValue>(m_pDevice);
	getMixerOutputControlValue->setPortId(m_iPortId);
	getMixerOutputControlValue->setMixerOutputNumber(
		m_pMixerInputParm->getAudioSourcePortId());
	getMixerOutputControlValue->setMixerInputNumber(m_iChannelNumber);
	// getMixerInputControlValue->setMixerInputNumber(1);

	try {
		m_pretSetMixerOutputControlValue =
			std::dynamic_pointer_cast<RetSetMixerOutputControlValue>(
				getMixerOutputControlValue->querySmart());
	} catch (ProtocolException protocolException) {
		std::cerr << protocolException.getErrorMessage() << std::endl;
	} catch (std::out_of_range oor) {
		std::cerr << oor.what() << std::endl;
	}*/
}

void AudioMixerOutputChannelWidget::changeOutput(
	unsigned int audioSinkPortId, std::string audioSinkPortName,
	unsigned int audioSinkChannelNumber, std::string audioSinkChannelName,
	AudioPortType audioPortType) {
	setOutputName(audioSinkPortId, audioSinkPortName, audioSinkChannelNumber,
				  audioSinkChannelName, audioPortType);
}

void AudioMixerOutputChannelWidget::changeMeterVolume(unsigned int channel,
													  int value) {
	if (channel == m_iChannelNumber) {
		ui->m_pPBLeft->setValue(value);
	}
	if (m_bIsMaster && channel == m_iChannelNumber + 1)
		ui->m_pPBRight->setValue(value);
}
